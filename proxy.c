/*
 * gaim
 *
 * Copyright (C) 1998-1999, Mark Spencer <markster@marko.net>
 * Copyright (C) 2002-2004, Wilmer van der Gaast, Jelmer Vernooij
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#define BITLBEE_CORE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "proxy.h"

char proxyhost[128] = "";
int proxyport = 0;
int proxytype = PROXY_NONE;
/* Some systems don't know this one. It's not essential, so set it to 0 then. */
#ifndef AI_NUMERICSERV
#define AI_NUMERICSERV 0
#endif
#ifndef AI_ADDRCONFIG
#define AI_ADDRCONFIG 0
#endif

static GHashTable *phb_hash = NULL;

struct PHB {
	b_event_handler func, proxy_func;
	gpointer data, proxy_data;
	char *host;
	int port;
	int fd;
	gint inpa;
	struct addrinfo *gai, *gai_cur;
};

void closesocket(int fd)
{
	close(fd);
}

typedef int (*proxy_connect_func)(const char *host, unsigned short port_, struct PHB *phb);

static int proxy_connect_none(const char *host, unsigned short port_, struct PHB *phb);

static gboolean phb_free(struct PHB *phb, gboolean success)
{
	g_hash_table_remove(phb_hash, &phb->fd);

	if (!success) {
		if (phb->fd > 0) {
			closesocket(phb->fd);
		}
		if (phb->func) {
			phb->func(phb->data, -1, B_EV_IO_READ);
		}
	}
	if (phb->gai) {
		freeaddrinfo(phb->gai);
	}
	g_free(phb->host);
	g_free(phb);
	return FALSE;
}

static gboolean proxy_connected(gpointer data, gint source, b_input_condition cond)
{
	struct PHB *phb = data;
	socklen_t len;
	int error = ETIMEDOUT;

	len = sizeof(error);

	if (getsockopt(source, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error) {
		if ((phb->gai_cur = phb->gai_cur->ai_next)) {
			int new_fd;
			b_event_remove(phb->inpa);
			if ((new_fd = proxy_connect_none(NULL, 0, phb))) {
				b_event_remove(phb->inpa);
				closesocket(source);
				dup2(new_fd, source);
				closesocket(new_fd);
				phb->fd = source;
				phb->inpa = b_input_add(source, B_EV_IO_WRITE, proxy_connected, phb);
				return FALSE;
			}
		}
		closesocket(source);
		source = -1;
		/* socket is dead, but continue to clean up */
	} else {
		sock_make_blocking(source);
	}

	freeaddrinfo(phb->gai);
	phb->gai = NULL;

	b_event_remove(phb->inpa);
	phb->inpa = 0;

	if (phb->proxy_func) {
		phb->proxy_func(phb->proxy_data, source, B_EV_IO_READ);
	} else {
		phb->func(phb->data, source, B_EV_IO_READ);
		// phb_free(phb, TRUE);
	}

	return FALSE;
}

static int proxy_connect_none(const char *host, unsigned short port_, struct PHB *phb)
{
	struct sockaddr_in me;
	int fd = -1;

	if (phb->gai_cur == NULL) {
		int ret;
		char port[6];
		struct addrinfo hints;

		g_snprintf(port, sizeof(port), "%d", port_);

		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;

		if (!(ret = getaddrinfo(host, port, &hints, &phb->gai))) {
			phb->gai_cur = phb->gai;
		} else {
			event_debug("gai(): %s\n", gai_strerror(ret));
		}
	}

	for (; phb->gai_cur; phb->gai_cur = phb->gai_cur->ai_next) {
		if ((fd = socket(phb->gai_cur->ai_family, phb->gai_cur->ai_socktype, phb->gai_cur->ai_protocol)) < 0) {
			event_debug("socket failed: %d\n", errno);
			continue;
		}

		sock_make_nonblocking(fd);

#if 0
		if (global.conf->iface_out) {
			me.sin_family = AF_INET;
			me.sin_port = 0;
			me.sin_addr.s_addr = inet_addr(global.conf->iface_out);

			if (bind(fd, (struct sockaddr *) &me, sizeof(me)) != 0) {
				event_debug("bind( %d, \"%s\" ) failure\n", fd, global.conf->iface_out);
			}
		}
#endif

		event_debug("proxy_connect_none( \"%s\", %d ) = %d\n", host, port_, fd);

		if (connect(fd, phb->gai_cur->ai_addr, phb->gai_cur->ai_addrlen) < 0 && !sockerr_again()) {
			event_debug("connect failed: %s\n", strerror(errno));
			closesocket(fd);
			fd = -1;
			continue;
		} else {
			phb->inpa = b_input_add(fd, B_EV_IO_WRITE, proxy_connected, phb);
			phb->fd = fd;

			break;
		}
	}

	if (fd < 0 && host) {
		phb_free(phb, TRUE);
	}

	return fd;
}


/* Connecting to HTTP proxies */

#define HTTP_GOODSTRING "HTTP/1.0 200"
#define HTTP_GOODSTRING2 "HTTP/1.1 200"

/* Export functions */

int proxy_connect(const char *host, int port, b_event_handler func, gpointer data)
{
	struct PHB *phb;
	int fd;

	if (!phb_hash) {
		phb_hash = g_hash_table_new(g_int_hash, g_int_equal);
	}

	if (!host || port <= 0 || !func || strlen(host) > 128) {
		return -1;
	}

	phb = g_new0(struct PHB, 1);
	phb->func = func;
	phb->data = data;


	fd = proxy_connect_none(host, port, phb);

	if (fd != -1) {
		g_hash_table_insert(phb_hash, &phb->fd, phb);
	}

	return fd;
}

void proxy_disconnect(int fd)
{
	struct PHB *phb = g_hash_table_lookup(phb_hash, &fd);

	if (!phb) {
		/* not in the early part of the connection - just close the fd */
		closesocket(fd);
		return;
	}

	if (phb->inpa) {
		b_event_remove(phb->inpa);
		phb->inpa = 0;
	}

	/* avoid calling the callback, which might result in double-free */
	phb->func = NULL;

	/* close and free */
	phb_free(phb, FALSE);
}
