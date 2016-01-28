
#include "irc_downstream.h"
#include "brine_glue.h"
#include "data_glue.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct command {
  const char *name;
  int params;
  void (*exec)(struct im_connection *ic, char **argv);
};

static void irc_cmd_ping(struct im_connection *ic, char **argv) {

}

/* Simplified PRIVMSG handling. One target, etc. */
static void irc_cmd_privmsg(struct im_connection *ic, char ** argv) {
  if (!argv[2]) {
    imcb_lognum(ic, 412, ":No text to send!");
    return;
  }

  /* Don't treat CTCP actions as real CTCPs, just convert them right now. */
  if (g_strncasecmp(argv[2], "\001ACTION", 7) == 0) {
    argv[2] += 4;
    memcpy(argv[2], "/me", 3);
    if (argv[2][strlen(argv[2]) - 1] == '\001') {
      argv[2][strlen(argv[2]) - 1] = '\0';
    }
  }

  if (argv[1][0] == '#') {
    /* Sending to a channel */
    if (argv[2][0] == '\001') {
      imcb_log(ic, "Sending CTCP to channels over libbrine is not supported yet.");
      /* CTCPs to channels? Nah. Maybe later. */
    } else {
      struct groupchat *c = bee_chat_by_title(ic->bee, ic, argv[1]);
      if (!c) {
        imcb_error(ic, "Could not find channel! Unless you were just screwing around, or deliberately sending to a channel you're not in, this shouldn't happen. Please tell the developer!");
        return;
      }
      ic->acc->prpl->chat_msg(c, argv[2], 0);
    }
  } else if (imcb_buddy_by_handle(ic, argv[1])) {
    /* Sending to a user */
    if (argv[2][0] == '\001') {
      imcb_log(ic, "Sending CTCP to users over libbrine is not supported yet.");
#if 0
      char **ctcp;

      if (iu->f->ctcp == NULL) {
        return;
      }
      if (cmd[2][strlen(cmd[2]) - 1] == '\001') {
        cmd[2][strlen(cmd[2]) - 1] = '\0';
      }

      ctcp = split_command_parts(cmd[2] + 1, 0);
      iu->f->ctcp(iu, ctcp);
#endif
    } else {
      ic->acc->prpl->buddy_msg(ic, argv[1], argv[2], 0);
    }
  } else {
    imcb_lognum(ic, 401, "%s :No such nick/channel", argv[1]);
  }
}

static int sorted = 0;
static struct command irc_commands[] = {
  // {"ping", 0, irc_cmd_ping}
  { "privmsg", 2, irc_cmd_privmsg }
};

/* Split an IRC-style line into little parts/arguments. */
static char **irc_parse_line(char *line)
{
	int i, j;
	char **cmd;

	/* Move the line pointer to the start of the command, skipping spaces and the optional prefix. */
	if (line[0] == ':') {
		for (i = 0; line[i] && line[i] != ' '; i++) {
			;
		}
		line = line + i;
	}
	for (i = 0; line[i] == ' '; i++) {
		;
	}
	line = line + i;

	/* If we're already at the end of the line, return. If not, we're going to need at least one element. */
	if (line[0] == '\0') {
		return NULL;
	}

	/* Count the number of char **cmd elements we're going to need. */
	j = 1;
	for (i = 0; line[i] != '\0'; i++) {
		if (line[i] == ' ') {
			j++;

			if (line[i + 1] == ':') {
				break;
			}
		}
	}

	/* Allocate the space we need. */
	cmd = g_new(char *, j + 1);
	cmd[j] = NULL;

	/* Do the actual line splitting, format is:
	 * Input: "PRIVMSG #bitlbee :foo bar"
	 * Output: cmd[0]=="PRIVMSG", cmd[1]=="#bitlbee", cmd[2]=="foo bar", cmd[3]==NULL
	 */

	cmd[0] = line;
	for (i = 0, j = 0; line[i] != '\0'; i++) {
		if (line[i] == ' ') {
			line[i] = '\0';
			cmd[++j] = line + i + 1;

			if (line[i + 1] == ':') {
				cmd[j]++;
				break;
			}
		}
	}

	return cmd;
}

void plugin_command_add(struct prpl *plugin, const char *name, int params, void (*f)(struct im_connection *ic, char **args)) {
  if (plugin->cmd_num == plugin->cmd_max) {
    plugin->cmds = realloc(plugin->cmds, (plugin->cmd_max += 5) * (sizeof(void (*)(struct im_connection *ic, char **argv))));
  }
  plugin->cmds[plugin->cmd_num++] = (struct command) { .name = name, .params = params, .exec = f };
}

static int command_compare(const void *vl, const void *vr) {
  const struct command *l = vl, *r = vr;
  return g_strcasecmp(l->name, r->name);
}

void commands_finalize(void *base, size_t ncommands) {
  qsort(base, ncommands, sizeof(struct command), command_compare);
}

void irc_downstream_putline(struct im_connection *ic, char *line) {
  char **argv = irc_parse_line(line);
  if (!sorted) {
    fprintf(stderr, "Sorting %zd basic commands\n", sizeof(irc_commands)/sizeof(struct command));
    qsort(irc_commands, sizeof(irc_commands)/sizeof(struct command), sizeof(struct command), command_compare);
  }
  struct command tmpl = {.name = argv[0]};
  struct command *cmd;
  if (ic->acc->prpl->cmd_num) {
    cmd = bsearch(&tmpl, ic->acc->prpl->cmds, ic->acc->prpl->cmd_num, sizeof(struct command), command_compare);
    if (cmd) {
      cmd->exec(ic, argv);
      g_free(argv);
      return;
    }
  }
  cmd = bsearch(&tmpl, irc_commands, sizeof(irc_commands)/sizeof(struct command), sizeof(struct command), command_compare);
  if (cmd) {
    cmd->exec(ic, argv);
  } else {
    imcb_log(ic, "Ignoring unknown command '%s'!", argv[0]);
  }
  g_free(argv);
}
