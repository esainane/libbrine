
#include "sha1.h"

#include "log.h"

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <glib.h>

void sha1_init(sha1_state_t *ctx)
{
	*ctx = g_checksum_new(G_CHECKSUM_SHA1);
}


void sha1_append(sha1_state_t *ctx, const guint8 * message_array, guint len)
{
	g_checksum_update(*ctx, message_array, len);
}

static void sha1_finish(sha1_state_t *ctx, guint8 digest[SHA1_HASH_SIZE])
{
	gsize digest_len = SHA1_HASH_SIZE;

	g_checksum_get_digest(*ctx, digest, &digest_len);
	g_checksum_free(*ctx);
}


/* I think this follows the scheme described on:
   http://en.wikipedia.org/wiki/Universally_unique_identifier#Version_4_.28random.29
   My random data comes from a SHA1 generator but hey, it's random enough for
   me, and RFC 4122 looks way more complicated than I need this to be.

   Returns a value that must be free()d. */
char *sha1_random_uuid(sha1_state_t * context)
{
	guint8 dig[SHA1_HASH_SIZE];
	char *ret = g_new0(char, 40);   /* 36 chars + \0 */
	int i, p;

	sha1_finish(context, dig);
	for (p = i = 0; i < 16; i++) {
		if (i == 4 || i == 6 || i == 8 || i == 10) {
			ret[p++] = '-';
		}
		if (i == 6) {
			dig[i] = (dig[i] & 0x0f) | 0x40;
		}
		if (i == 8) {
			dig[i] = (dig[i] & 0x30) | 0x80;
		}

		sprintf(ret + p, "%02x", dig[i]);
		p += 2;
	}
	ret[p] = '\0';

	return ret;
}

/* A wrapper for /dev/urandom.
 * If /dev/urandom is not present or not usable, it calls abort()
 * to prevent bitlbee from working without a decent entropy source */
void random_bytes(unsigned char *buf, int count)
{
	int fd;

	if (((fd = open("/dev/urandom", O_RDONLY)) == -1) ||
	    (read(fd, buf, count) == -1)) {
		log_message(LOGLVL_ERROR, "/dev/urandom not present - aborting");
		abort();
	}

	close(fd);
}
