/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:52:16 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/11 19:38:32 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                            INCLUDE ALL LIBRARY                             */
/* ************************************************************************** */

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <wait.h>

/* ************************************************************************** */
/*                            DEFINE ALL CONSTANT                             */
/* ************************************************************************** */

# define BLACK "\1\e[1;90m\2"
# define RED "\1\e[1;91m\2"
# define GREEN "\1\e[1;92m\2"
# define YELLOW "\1\e[1;93m\2"
# define BLUE "\1\033[38;5;117m\2"
# define PURPLE "\1\033[38;5;164m\2"
# define CYAN "\1\033[38;5;51m\2"
# define WHITE "\1\e[1;97m\2"
# define RESET "\1\e[0m\2"
# define BOLD "\1\e[1m\2"
# define GREY "\033[90m\2"
# define LILA "\1\033[38;5;147m\2"
# define O_BLUE "\1\033[38;5;24m\2"
# define TEAL "\1\033[38;5;44m\2"
# define LIGHT_GREEN "\1\033[38;5;120m\2"

# define E_MALLOC "Error: malloc failed : "
# define E_ARGS "Error: too many arguments\n"
# define E_SYNTAX "syntax error near unexpected token "
# define E_HEREDOC "\nhere-document delimited by end-of-file wanted "
# define E_PIPE "Error: pipe failed\n"
# define E_REDIR "ambiguous redirect : "
# define E_EOF "unexpected EOF while looking for matching "
# define E_S_EOF "syntax error : unexpected EOF\n"
# define E_DIR_ERROR "error retrieving current directory: "
# define E_GETCWD_ERROR "getcwd: cannot access parent directories:"
# define E_EXIT "exit: "
# define E_EXIT_MANY "too many arguments\n"
# define E_EXIT_NUM "numeric argument required\n"
# define E_EXPORT "export: "
# define E_2 ": "
# define E_EXPORT_ID "not a valid identifier"
# define E_CD "cd: "
# define E_ENV "env: "

/* ************************************************************************** */
/*                            DEFINE ALL STRUCTURE                            */
/* ************************************************************************** */
typedef enum e_token
{
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HERE_DOC,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	SPACES,
	CHAR,
	CMD,
	QUOTE_CMD,
	DOL,
	OR,
	AND,
	PARENTH_OPEN,
	PARENTH_CLOSE,
	WILDCARD,
	END
}					t_token;

typedef struct s_env
{
	char			*line;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

typedef struct s_word
{
	bool			here_doc_expand;
	bool			in_quote;
	int				token;
	char			*word;
	struct s_word	*prev;
	struct s_word	*next;
}					t_word;

typedef struct s_var
{
	int				here_doc_count;
	int				(*tab_builtins[8])(char **, struct s_var *);
	pid_t			last_pid;
	char			*line;
	char			**envp;
	bool			exit;
	bool			error;
	bool			execute_next;
	bool			bonus_cmd;
	bool			uncommitted_changes;
	bool			in_fork;
	bool			in_parenth;
	bool			git;
	char			*prompt;
	t_word			*lexer;
	t_word			*quoted_cmds;
	t_word			*tmp;
	t_env			*env;
}					t_var;

typedef struct index
{
	int				i;
	int				j;
	int				last_star_i;
	int				last_star_j;
}					t_index;

/* ************************************************************************** */
extern int			g_exit_status;

/*                            FUNCTION PROTOTYPES                             */

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

/* ********************************* MAIN.C ********************************* */

void				sigint_handler(int sig);
void				sigint_handler_child(int sig);
void				set_signals(void);
void				sigint_handler_here_doc(int signum);

/* ****************************** MAIN_UTILS.C ****************************** */

char				*get_git_branch(t_var *var);
void				get_line(t_var *var);
t_var				*init_var(t_env **envs);
void				init_tab_builtins(t_var **var);
char				*get_prompt(t_var *var);
char				*get_prompt_two(t_var *var, char *tmp2, char *git_branch);
char				*get_pwd(t_var *var);

/* ************************************************************************** */
/*                                      ENV                                   */
/* ************************************************************************** */

/* ********************************* ENV.C ********************************* */

void				add_node_env(t_env **envs, char *str);
char				**split_env(t_env *env);
void				init_env(t_env **envs, char **env);

/* ****************************** ENV_UTILS.C ******************************* */

t_env				*get_last_tenv(t_env *env);
int					count_node_env(t_env *env);

/* ************************************************************************** */
/*                                   PARSING                                  */
/* ************************************************************************** */

/* ******************************** PARSING.C ******************************* */

int					add_word(t_word **word, int token, char *str);
void				handle_token(t_var *var, int *tab, int i);
void				init_tab_token_2(char *line, int *tab, int *i);
int					*init_tab_token(char *line, int i);
void				parsing(t_var *var);

/* ****************************** HANDLE_ONE.C ****************************** */

void				handle_pipe(t_var *var, int *i);
void				handle_redir_in(t_var *var, int *i);
void				handle_redir_out(t_var *var, int *i);
void				handle_quotes(t_var *var, int *i, int token);
void				handle_char(t_var *var, int *i, int *tab);

/* ****************************** HANDLE_TWO.C ****************************** */

void				handle_space(t_var *var, int *i);
void				handle_dol(t_var *var, int *i, int *tab);
void				handle_or_and(t_var *var, int *i, int *tab);
void				handle_parent(t_var *var, int *i, int *tab);
void				handle_end(t_var *var);

/* ******************************* WILDCARD.C ******************************* */

void				do_wildcard(t_var *var);
void				del_start(t_word **tmp, t_word **start, t_var *var);
int					expand_wildcard(t_word **start, char *pattern, int *tab,
						t_var *var);
int					join_and_check_err(int i, t_word **start, t_word **new,
						t_var *var);
int					expand_it(struct dirent *file, t_word **new, t_var *var);
void				join_t_word(t_word **start, t_word **new);
bool				is_expandable(char *file, char *pattern, int *tab);
void				handle_index(t_index *index, int cas);
void				del_this_shit(t_word **start);
int					*init_tab_wildcard(t_word *start, t_var *var);
t_word				*find_start(t_word *tmp);

/* ****************************** CHECK_SYNTAX.C **************************** */

void				check_syntax(t_var *var);

/* *************************** CHECK_SYNTAX_PIPE.C ************************** */

int					check_pipe(t_word *tmp, bool dir);
int					check_syntax_pipe(t_word *lexer, t_var *var);

/* *************************** CHECK_SYNTAX_REDIR.C ************************* */

bool				check_token(t_word *tmp, t_word *start, t_var *var);
t_word				*check_and_trim(t_word *start, t_var *var);
int					check_syntax_redir(t_var *var);
void				join_if_need(t_word *head, t_var *var, int token);
char				*ft_strjoin_tword(t_word *tmp, t_var *var, int token);

/* *************************** CHECK_SYNTAX_OR_AND.C ************************ */

int					is_cmd(int token);
int					check_syntax_or_and(t_word *lexer, t_var *var);
int					check_syntax_parenth(t_word *lexer, t_var *var);
t_word				*end_of_parenth(t_word *lexer);
int					check_before_and_affter(t_word *tmp, bool dir);
int					check_parenth_close(t_word *tmp, t_var *var);
int					check_out_pareth(t_word *lexer);
int					check_in_parenth2(t_word *tmp, bool cmd, bool parenthsis);
int					check_in_parenth(t_word *lexer);

/* ************************** CHECK_SYNTAX_UTILS.C ************************** */

int					ft_strlen_tword(t_word *tmp, int token);
void				trim_tword(t_word **start, t_word **end);
t_word				*trim_rout(t_word *start, t_var *var);
void				handle_token_logic(t_word *tmp, t_var *var, int token,
						t_word *head);
bool				can_i_join(t_word *tmp, int token);

/* ***************************** PARSING_UTILS.C **************************** */

char				*ft_strndup(char *line, size_t end, size_t start);
t_word				*get_last_tword(t_word *word);
void				del_tword(t_word **word);
t_word				*skip_token(t_word *tmp, int token, bool dir);
int					ft_isgoodchar(char c);

/* ************************************************************************** */
/*                                   EXPAND                                   */
/* ************************************************************************** */

/* ******************************** EXPAND.C ******************************** */

void				replace_dol(t_word *tmp, char *str);
char				*find_in_env(char *str, t_var *var);
void				find_and_replace(t_word *tmp, t_var *var);
void				init_quoted_cmd(t_word *tmp, t_var *var);
void				expand(t_var *var);

/******************************* HANDLE_EXPAND.C *****************************/

void				handle_quoted_space(t_var *var, int *i, char *line);
void				handle_quoted_char(t_var *var, int *i, int *tab,
						char *line);
void				handle_quoted_dol(t_var *var, int *i, int *tab, char *line);
void				handle_quoted_end(t_var *var);
void				handle_quoted_token(t_var *var, int *tab, int i,
						char *line);

/******************************* EXPAND_UTILS.C ******************************/

int					len_quoted_cmd(t_var *var);
char				*join_quoted_cmd(t_var *var);
void				fill_tab(t_var *var, char **word);
void				expand_quoted_cmds(t_var *var);
char				*expand_here_doc_line(char *line, t_var *var);

/****************************************************************************/
/*                                  EXECUTE                                 */
/****************************************************************************/

/******************************* BEFOR_EXEX.C *******************************/

void				loop_here_doc(char *eof, int fd, t_word *tmp, t_var *var);
int					here_doc(t_word *tmp, t_var *var, char *file_name);
void				do_here_doc(t_var *var);
void				before_exe(t_var *var);
void				fork_here_doc(t_var *var, t_word *tmp, char *file_name);
void				join_node2(t_word *head, t_var *var);
void				join_node(t_var *var);

/********************************** EXEX.C **********************************/

void				exec(char **cmd, char **env);
void				child(int c_fd, int pipe_fd[2], int i, t_var *var);
t_word				*cmd_found(t_word *lexer);
int					fork_loop(t_var *var, int nb_cmd);
void				exe_cmd(t_var *var);

/********************************* EXEX_BONUS_CMD.C *************************/
int					is_bonus_cmd(t_word *lexer);
void				new_lst(t_word **start, t_word **lexer, t_word *curent);

bool				update_execution_state(t_word **start, t_word *tmp,
						t_var *var, bool flag);
t_word				*update_tmp(t_word *tmp, t_var *var);
int					do_bonus_cmd(int c_fd, int pipe_fd[2], int i, t_var *var);
t_word				*find_token(t_word *lexer, int token);
void				do_cmd_in_parenth(int c_fd, int pipe_fd[2], int i,
						t_var *var);
void				need_to_wait(t_word *tmp, pid_t pid);
int					can_i_run_without_fork(t_var *var, t_word *tmp,
						t_word *head, int i);
void				run_without_fork(t_var *var, t_word **head, int c_fd,
						int pipe_fd[2]);
bool				run_in_fork(t_var *var, t_word *tmp, bool flag);

/*********************************** DUP.C **********************************/

void				do_dup_in2(int pipe_fd[2], int c_fd, t_var *var);
void				do_dup_in(int pipe_fd[2], int flag[4], t_word *tmp,
						t_var *var);
void				do_dup_out2(int pipe_fd[2], t_word *tmp, t_var *var);
void				do_dup_out(int pipe_fd[2], int flag[4], t_word *tmp,
						t_var *var);
void				do_dup(int c_fd, int pipe_fd[2], int i, t_var *var);

/********************************* SPLIT_CMD.C *******************************/

int					count_cmd(t_word *node);
int					ft_strlen_node(t_word *word);
char				*ft_strjoin_node(t_word *word);
t_word				*next_word(t_word *word);
char				**split_cmd(t_var *var);

/********************************* EXEX_UTILS.C ******************************/

void				del_cmd(t_word **word);
int					is_dir(char *cmd);
void				error_msg(char *path, char **cmd, char **env);
char				*get_path(char **cmd, char **path);
int					wait_for_child(pid_t pid);

/******************************* EXEX_UTILS_TWO.C ****************************/

int					node_cmp_token(t_word *lexer, int token);
void				close_fd(int fd, int i);
void				close_all_fd(int pipe_fd[2], int c_fd, int i,
						bool close_fd_0);
int					do_simple_cmd(int c_fd, int pipe_fd[2], int i, t_var *var);
int					choose_how_to_exec(int c_fd, int pipe_fd[2], int i,
						t_var *var);

/****************************************************************************/
/*                                FREE_AND_EXIT                             */
/****************************************************************************/

/********************************** FREE.C **********************************/

void				free_list_lexer(t_word **lexer);
void				free_list_env(t_env **env);
void				free_var(t_var *var);
void				free_error(t_var *var, char *error, char *fautif, int ff);
void				free_split(char **tab);

/********************************** FREE.C **********************************/
void				unlink_here_doc(t_var *var);

/********************************** EXIT.C **********************************/

void				check_exit(t_var *var);
int					get_terminal_width(void);
void				print_exit_status(int width, bool is_error);

/********************************* BUILTINS *********************************/
int					do_bultins(t_var *var);
int					is_builtins(t_word *tmp);
void				error_msg_builtins(char *builtin, char *fautife, char *err);
int					cd(char **cmd, t_var *var);
int					pwd(char **cmd, t_var *var);
int					export(char **cmd, t_var *var);
int					unset(char **cmd, t_var *var);
int					echo(char **cmd, t_var *var);
int					env(char **cmd, t_var *var);
int					exit_builtin(char **cmd, t_var *var);
int					replace_env(t_var *var, char *str, char *new_env_value);
int					replace_in_env(t_var *var);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_chdir(char *path, t_var *var, char *env);
int					find_replace_env(t_env *envp, char *str, int len,
						char *tmp);
int					do_dup_in_builtins(t_word *tmp);
int					do_dup_out_builtins(t_word *tmp);
int					do_dup_builtins(t_word *tmp, int *old_fd_in,
						int *old_fd_out);
int					restore_fds(int old_fd_in, int old_fd_out);
int					is_dir_in_cmd(t_word *tmp);
t_env				*find_smallest_node(t_env *head);
t_env				*find_next_smallest_node(t_env *head,
						t_env *current_smallest);
void				print_env_ordered(t_env *env_list, t_var *var);
int					is_valid_identifier(char *str);
void				add_variable_in_env(t_var *var, char *tmp);
void				choose_how_to_add(t_var *var, char *cmd);

/****************************************************************************/
/*                                  SIGNAL                                  */
/****************************************************************************/

/********************************* SIGNAL.C *********************************/

void				sigint_handler_child(int sig);

/****************************************************************************/

void				count_node(t_word *word);
// void				print_list(t_word *word);
void				print_list_env(t_env *env);

#endif
