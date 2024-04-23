/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:52:16 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/23 15:34:32 by pscala           ###   ########.fr       */
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

# define PROMPT "minishell$ "
# define E_Malloc "Error: malloc failed : "
# define E_args "Error: too many arguments\n"
# define E_syntax "syntax error near unexpected token "
# define E_pipe "Error: pipe failed\n"

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
	t_word			*lexer;
	t_env			*env;
}					t_var;

/* ************************************************************************** */
/*                            FUNCTION PROTOTYPES                             */
/* ************************************************************************** */

/* ********************************* MAIN.C ********************************* */

// int					main(int ac, char **av, char **env);

/* ******************************** PARSING.C ******************************** */

void				parsing(t_var *var);

/* ***************************** CHECK_SYNSTAX.C ***************************** */

void				check_syntax(t_var *var);
void				del_tword(t_word **word);

/* ********************************* UTILS.C ******************************** */

void				init_var(t_var *var, t_env **envs);
char				*ft_strndup(char *line, int i, int start);
void				print_list(t_word *word);
void				count_node(t_word *word);
void				print_list_env(t_env *env);
int					node_cmp_token(t_word *lexer, int token);
int					count_node_token(t_word *lexer, int token);
int					count_node_env(t_env *env);
char				**split_env(t_env *env);
void				del_cmd(t_word **word);
void				close_fd(int fd, int i);

/* ******************************** GLNODE.C ******************************** */

t_word				*get_last_tword(t_word *word);
t_env				*get_last_tenv(t_env *env);

/* ********************************* TEST.C ********************************* */

/* ********************************* ENV.C ********************************** */

void				init_env(t_env **envs, char **env);
void				add_node_env(t_env **envs, char *str);

/* ********************************* FREE.C ********************************* */
void				free_var(t_var *var);
void				free_list_env(t_env **env);
void				free_list_lexer(t_word **lexer);
void				free_error(t_var *var, char *error, char *fautif, int ff);
void				free_split(char **tab);

/* ******************************** BEFORE_EXEC.C ***************************** */

void				before_exe(t_var *var);

/* *********************************** EXEC.C ******************************** */
void				exec(char **cmd, char **env);

/* ****************************** SPLIT_CMD.C ****************************** */

char				**split_cmd(t_var *var);
int					count_cmd(t_word *node);

#endif