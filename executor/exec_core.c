// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   exec_core.c                                        :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/09/07 16:55:00 by leia              #+#    #+#             */
// /*   Updated: 2025/09/15 22:57:09 by leiwang          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../include/executor.h"

// static const char *get_env(const char *key, char **envp)
// {
//     size_t klen;
//     int    i;

//     if (!key || !envp) return NULL;
//     klen = ft_strlen(key);
//     for (i = 0; envp[i]; ++i)
//     {
//         if (!strncmp(envp[i], key, klen) && envp[i][klen] == '=')
//             return envp[i] + klen + 1;
//     }
//     return NULL;
// }

// static int is_exe_file(const char *path)
// {
//     struct stat st;
//     if (stat(path, &st) != 0)
//         return 0;
//     if (!S_ISREG(st.st_mode))
//         return 0;
//     if (access(path, X_OK) != 0)
//         return 0;
//     return 1;
// }

// static char *join_path(const char *dir, const char *file)
// {
//     size_t dl = ft_strlen(dir);
//     size_t fl = ft_strlen(file);
//     char *res = malloc(dl + 1 + fl + 1);
//     if (!res) return NULL;
//     memcpy(res, dir, dl);
//     res[dl] = '/';
//     memcpy(res + dl + 1, file, fl);
//     res[dl + 1 + fl] = '\0';
//     return res;
// }

// static char *find_in_path(const char *file, char **envp)
// {
//     const char *path = get_env("PATH", envp);
//     char *copy, *tok, *save;
//     char *cand = NULL;

//     if (!path || !*path)
//         return NULL;
//     copy = strdup(path);
//     if (!copy) return NULL;
//     save = NULL;
//     for (tok = strtok_r(copy, ":", &save); tok; tok = strtok_r(NULL, ":", &save))
//     {
//         cand = join_path(tok, file);
//         if (!cand) { free(copy); return NULL; }
//         if (is_exe_file(cand))
//         {
//             free(copy);
//             return cand;
//         }
//         free(cand);
//         cand = NULL;
//     }
//     free(copy);
//     return NULL;
// }

// static int apply_redirs_in_child(t_redir *r)
// {
//     int fd;
//     while (r)
//     {
//         if (r->redir_type == R_REDIR_IN)
//         {
//             fd = open(r->file, O_RDONLY);
//             if (fd < 0) return ft_perror(r->file);
//             if (dup2(fd, STDIN_FILENO) < 0) { int e=errno; close(fd); errno=e; return ft_perror("dup2"); }
//             close(fd);
//         }
//         else if (r->redir_type == R_REDIR_OUT)
//         {
//             fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (fd < 0) return ft_perror(r->file);
//             if (dup2(fd, STDOUT_FILENO) < 0) { int e=errno; close(fd); errno=e; return ft_perror("dup2"); }
//             close(fd);
//         }
//         else if (r->redir_type == R_REDIR_APPEND)
//         {
//             fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (fd < 0) return ft_perror(r->file);
//             if (dup2(fd, STDOUT_FILENO) < 0) { int e=errno; close(fd); errno=e; return ft_perror("dup2"); }
//             close(fd);
//         }
//         else if (r->redir_type == R_REDIR_HEREDOC)
//         {
//             if (r->fd >= 0)
//             {
//                 if (dup2(r->fd, STDIN_FILENO) < 0) { return ft_perror("dup2"); }
//                 close(r->fd);
//                 r->fd = -1;
//             }
//         }
//         r = r->next;
//     }
//     return 0;
// }

// static int map_exec_error_to_status(int err)
// {
//     if (err == ENOENT)
//         return 127; /* command not found */
//     if (err == EACCES || err == EPERM || err == ENOEXEC)
//         return 126; /* permission problem or not executable */
//     return 127;
// }

// int exec_single_external(t_cmd *node, char **envp)
// {
//     pid_t pid;
//     int   st;

//     if (!node || !node->argv || !node->argv[0])
//         return 0;
//     pid = fork();
//     if (pid < 0)
//         return ft_perror("fork");
//     if (pid == 0)
//     {
//         /* Child: set default signals */
//         signal(SIGINT, SIG_DFL);
//         signal(SIGQUIT, SIG_DFL);
//         if (apply_redirs_in_child(node->redirs) != 0)
//             _exit(1);

//         /* Resolve and exec */
//         const char *file = node->argv[0];
//         if (strchr(file, '/'))
//         {
//             execve(file, node->argv, envp);
//             _exit(map_exec_error_to_status(errno));
//         }
//         else
//         {
//             char *path = find_in_path(file, envp);
//             if (path)
//             {
//                 execve(path, node->argv, envp);
//                 int rc = map_exec_error_to_status(errno);
//                 free(path);
//                 _exit(rc);
//             }
//             /* not found */
//             dprintf(2, "minishell: %s: command not found\n", file);
//             _exit(127);
//         }
//     }
//     /* Parent */
//     if (waitpid(pid, &st, 0) == -1)
//         return ft_perror("waitpid");
//     if (WIFSIGNALED(st))
//         g_last_status = 128 + WTERMSIG(st);
//     else if (WIFEXITED(st))
//         g_last_status = WEXITSTATUS(st) & 0xFF;
//     return g_last_status;
// }

// int exec_pipeline(t_cmd *head, char **envp)
// {
//     int   count = 0;
//     t_cmd *it;
//     int  **pipes = NULL;
//     pid_t *pids = NULL;
//     int   i, st;

//     for (it = head; it; it = it->next) count++;
//     if (count <= 1)
//         return exec_single_external(head, envp);

//     pipes = calloc((size_t)(count - 1), sizeof(int*));
//     pids  = calloc((size_t)count, sizeof(pid_t));
//     if (!pipes || !pids)
//         return ft_perror("calloc");
//     for (i = 0; i < count - 1; ++i)
//     {
//         pipes[i] = calloc(2, sizeof(int));
//         if (!pipes[i]) return ft_perror("calloc");
//         if (pipe(pipes[i]) < 0) return ft_perror("pipe");
//     }

//     it = head;
//     for (i = 0; i < count; ++i)
//     {
//         pids[i] = fork();
//         if (pids[i] < 0)
//             return ft_perror("fork");
//         if (pids[i] == 0)
//         {
//             /* Child i */
//             signal(SIGINT, SIG_DFL);
//             signal(SIGQUIT, SIG_DFL);
//             if (i > 0)
//                 dup2(pipes[i-1][READ_END], STDIN_FILENO);
//             if (i < count - 1)
//                 dup2(pipes[i][WRITE_END], STDOUT_FILENO);
//             /* close all pipe fds */
//             for (int k = 0; k < count - 1; ++k)
//             {
//                 close(pipes[k][READ_END]);
//                 close(pipes[k][WRITE_END]);
//             }
//             if (apply_redirs_in_child(it->redirs) != 0)
//                 _exit(1);
//             if (is_builtin(it->argv && it->argv[0] ? it->argv[0] : NULL))
//             {
//                 /* Run builtin in child context */
//                 int rc = exec_builtin(it, envp);
//                 _exit(rc);
//             }
//             /* external */
//             const char *file = (it->argv && it->argv[0]) ? it->argv[0] : NULL;
//             if (!file)
//                 _exit(0);
//             if (strchr(file, '/'))
//             {
//                 execve(file, it->argv, envp);
//                 _exit(map_exec_error_to_status(errno));
//             }
//             else
//             {
//                 char *path = find_in_path(file, envp);
//                 if (path)
//                 {
//                     execve(path, it->argv, envp);
//                     int rc = map_exec_error_to_status(errno);
//                     free(path);
//                     _exit(rc);
//                 }
//                 dprintf(2, "minishell: %s: command not found\n", file);
//                 _exit(127);
//             }
//         }
//         /* Parent: close ends that are no longer needed */
//         if (i > 0)
//         {
//             close(pipes[i-1][READ_END]);
//             close(pipes[i-1][WRITE_END]);
//         }
//         it = it->next;
//     }
//     /* Close last pipe if exists */
//     if (count - 1 > 0)
//     {
//         close(pipes[count-2][READ_END]);
//         close(pipes[count-2][WRITE_END]);
//     }
//     /* Wait all */
//     for (i = 0; i < count; ++i)
//     {
//         if (waitpid(pids[i], &st, 0) == -1)
//             ft_perror("waitpid");
//         if (i == count - 1)
//         {
//             if (WIFSIGNALED(st))
//                 g_last_status = 128 + WTERMSIG(st);
//             else if (WIFEXITED(st))
//                 g_last_status = WEXITSTATUS(st) & 0xFF;
//         }
//     }
//     /* free pipes */
//     for (i = 0; i < count - 1; ++i)
//         free(pipes[i]);
//     free(pipes);
//     free(pids);
//     return g_last_status;
// }

