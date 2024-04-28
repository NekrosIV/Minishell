/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:52:16 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/28 18:03:53 by kasingh          ###   ########.fr       */
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
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <wait.h>

/* ************************************************************************** */
/*                            DEFINE ALL CONSTANT                             */
/* ************************************************************************** */
# define RED "\033[38;5;160m"
# define RESET "\033[00m"
# define PROMPT "minishell$ "
# define E_MALLOC "Error: malloc failed : "
# define E_ARGS "Error: too many arguments\n"
# define E_SYNTAX "syntax error near unexpected token "
# define E_PIPE "Error: pipe failed\n"
# define E_REDIR "ambiguous redirect : "

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
	int				token;
	char			*word;
	struct s_word	*prev;
	struct s_word	*next;
}					t_word;

typedef struct s_var
{
	char			*line;
	char			**envp;
	bool			exit;
	bool			error;
	int				status;
	t_word			*lexer;
	t_word			*quoted_cmds;
	t_env			*env;
}					t_var;

/*                            FUNCTION PROTOTYPES                             */

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

/* ********************************* MAIN.C ********************************* */

// int					main(int ac, char **av, char **env);

/* ****************************** MAIN_UTILS.C ****************************** */

t_var				*init_var(t_env **envs, int exit_statut);

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
void				init_tab_token_2(char *line, int *tab, int i);
int					*init_tab_token(char *line, int i);
void				parsing(t_var *var);

/* ****************************** HANDLE_ONE.C ****************************** */

void				handle_pipe(t_var *var, int *i);
void				handle_redir_in(t_var *var, int *i);
void				handle_redir_out(t_var *var, int *i);
void				handle_quotes(t_var *var, int *i);
void				handle_token(t_var *var, int *tab, int i);

/* ****************************** HANDLE_TWO.C ****************************** */

void				handle_char(t_var *var, int *i, int *tab);
void				handle_space(t_var *var, int *i);
void				handle_dol(t_var *var, int *i, int *tab);
void				handle_end(t_var *var);

/* ****************************** CHECK_SYNTAX.C **************************** */

void				check_syntax(t_var *var);

/* *************************** CHECK_SYNTAX_PIPE.C ************************** */

int					check_pipe(t_word *tmp, bool dir);
int					check_syntax_pipe(t_word *lexer, t_var *var);

/* *************************** CHECK_SYNTAX_REDIR.C ************************* */

bool				check_token(t_word *tmp, t_word *start, t_var *var);
t_word				*trim_rout(t_word *start, t_var *var);
t_word				*check_and_trim(t_word *start, t_var *var);
int					check_syntax_redir(t_var *var);

/* ***************************** PARSING_UTILS.C **************************** */

char				*ft_strndup(char *line, int end, int start);
t_word				*get_last_tword(t_word *word);
void				del_tword(t_word **word);
void				trim_tword(t_word **start, t_word **end);
t_word				*skip_token(t_word *tmp, int token);

/* ************************************************************************** */
/*                                   EXPAND                                   */
/* ************************************************************************** */

/* ******************************** EXPAND.C ******************************** */

void				replace_dol(t_word *tmp, char *str);
void				find_and_replace(t_word *tmp, t_var *var);
void				expand_quoted_cmds(t_var *var);
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

/****************************************************************************/
/*                                  EXECUTE                                 */
/****************************************************************************/

/******************************* BEFOR_EXEX.C *******************************/

void				loop_here_doc(char *eof, int fd);
int					here_doc(t_word *tmp, t_var *var);
void				do_here_doc(t_var *var);
void				before_exe(t_var *var);

/********************************** EXEX.C **********************************/

void				exec(char **cmd, char **env);
void				child(int c_fd, int pipe_fd[2], int i, t_var *var);
void				handle_fork_error(pid_t pid, int pipe_fd[2], int c_fd,
						int i);
int					fork_loop(t_var *var, int nb_cmd);
void				exe_cmd(t_var *var);

/*********************************** DUP.C **********************************/

void				do_dup_in(int pipe_fd[2], int c_fd, int flag[3],
						t_word *tmp);
void				do_dup_in2(int pipe_fd[2], int c_fd, int flag[3],
						t_word *tmp);

void				do_dup_out(int pipe_fd[2], int flag[3], t_word *tmp);
void				do_dup_out2(int pipe_fd[2], int flag[3], t_word *tmp);

void				do_dup(int c_fd, int pipe_fd[2], int i, t_var *var);

/********************************* SPLIT_CMD.C *******************************/

int					count_cmd(t_word *node);
int					ft_strlen_node(t_word *word);
char				*ft_strjoin_node(t_word *word);
t_word				*next_word(t_word *word);
char				**split_cmd(t_var *var);

/********************************* EXEX_UTILS.C ******************************/

void				del_cmd(t_word **word);
void				error_msg(char *path, char **cmd, char **env);
void				close_fd(int fd, int i);
char				*get_path(char **cmd, char **path);
int					wait_for_child(pid_t pid);

/******************************* EXEX_UTILS_TWO.C ****************************/

int					node_cmp_token(t_word *lexer, int token);

/****************************************************************************/
/*                                FREE_AND_EXIT                             */
/****************************************************************************/

/********************************** FREE.C **********************************/

void				free_list_lexer(t_word **lexer);
void				free_list_env(t_env **env);
void				free_var(t_var *var);
void				free_error(t_var *var, char *error, char *fautif, int ff);
void				free_split(char **tab);

/********************************** EXIT.C **********************************/

void				check_exit(t_var *var);

/****************************************************************************/

void				count_node(t_word *word);
void				print_list(t_word *word);
void				print_list_env(t_env *env);

#endif