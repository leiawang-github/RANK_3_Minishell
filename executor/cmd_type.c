#include "minishell.h"

static int  has_argv(const t_cmd *cmd)
{
	return (cmd->cmd_argv && cmd->cmd_argv[0]);
}

static int  has_assigns(const t_cmd *cmd)
{
	return (cmd->assigns != NULL);
}
static int  has_redirs(const t_cmd *cmd)
{
	return (cmd->redirs  != NULL);
}

void    classify_cmd(t_cmd *cmd)
{
    int a = has_argv(cmd);
    int b = has_assigns(cmd);
    int c = has_redirs(cmd);

    if (a)
        cmd->type = CMD_WITH_ARGV;
    else if (b && c)
        cmd->type = CMD_ASSIGN_AND_REDIR;
    else if (b && !c)
        cmd->type = CMD_ASSIGN_ONLY;
    else if (!b && c)
        cmd->type = CMD_REDIR_ONLY;
    else
        cmd->type = CMD_EMPTY;
}

