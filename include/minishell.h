/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:52:16 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/15 17:26:32 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                            INCLUDE ALL LIBRARY                             */
/* ************************************************************************** */

# include "../libft/libft.h"
# include <fcntl.h>
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

# define BLACK "\e[1;90m"
# define RED "\e[1;91m"
# define GREEN "\e[1;92m"
# define YELLOW "\e[1;93m"
# define BLUE "\033[38;5;117m"
# define PURPLE "\e[1;95m"
# define CYAN "\e[1;96m"
# define WHITE "\e[1;97m"
# define RESET "\e[0m"
# define BOLD "\e[1m"
# define GREY "\033[90m"

# define E_MALLOC "Error: malloc failed : "
# define E_ARGS "Error: too many arguments\n"
# define E_SYNTAX "syntax error near unexpected token "
# define E_HEREDOC "\nhere-document delimited by end-of-file wanted "
# define E_PIPE "Error: pipe failed\n"
# define E_REDIR "ambiguous redirect : "
# define E_EOF "unexpected EOF while looking for matching "
# define E_S_EOF "syntax error : unexpected EOF\n"

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
	int				token;
	char			*word;
	struct s_word	*prev;
	struct s_word	*next;
}					t_word;

typedef struct s_var
{
	int				here_doc_count;
	pid_t			last_pid;
	char			*line;
	char			**envp;
	bool			exit;
	bool			error;
	bool			execute_next;
	bool			bonus_cmd;
	bool			use_ls_alias;
	char			*prompt;
	t_word			*lexer;
	t_word			*quoted_cmds;
	t_word			*tmp;
	t_env			*env;
}					t_var;

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
// int					main(int ac, char **av, char **env);

/* ****************************** MAIN_UTILS.C ****************************** */

void				get_line(t_var *var);
t_var				*init_var(t_env **envs, bool allias_ls);

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

/* ****************************** CHECK_SYNTAX.C **************************** */

void				check_syntax(t_var *var);

/* *************************** CHECK_SYNTAX_PIPE.C ************************** */

int					check_pipe(t_word *tmp, bool dir);
int					check_syntax_pipe(t_word *lexer, t_var *var);

/* *************************** CHECK_SYNTAX_REDIR.C ************************* */

bool				check_token(t_word *tmp, t_word *start, t_var *var);
t_word				*check_and_trim(t_word *start, t_var *var);
int					check_syntax_redir(t_var *var);

/* *************************** CHECK_SYNTAX_OR_AND.C ************************ */

int					check_pipe(t_word *tmp, bool dir);
int					check_syntax_or_and(t_word *lexer, t_var *var);
int					check_syntax_parenth(t_word *lexer, t_var *var);
t_word				*end_of_parenth(t_word *lexer);

/* ************************** CHECK_SYNTAX_UTILS.C ************************** */

int					ft_strlen_tword(t_word *tmp);
void				trim_tword(t_word **start, t_word **end);
t_word				*trim_rout(t_word *start, t_var *var);
void				handle_token_logic(t_word *tmp, t_var *var, int token,
						t_word *head);

/* ***************************** PARSING_UTILS.C **************************** */

char				*ft_strndup(char *line, int end, int start);
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

/********************************** EXEX.C **********************************/

void				exec(char **cmd, char **env);
void				child(int c_fd, int pipe_fd[2], int i, t_var *var);
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

/****************************************************************************/
/*                                  SIGNAL                                  */
/****************************************************************************/

/********************************* SIGNAL.C *********************************/

void				sigint_handler_child(int sig);

/****************************************************************************/

void				count_node(t_word *word);
void				print_list(t_word *word);
void				print_list_env(t_env *env);

#endif
