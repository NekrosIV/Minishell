/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:52:16 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/16 16:01:05 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                            INCLUDE ALL LIBRARY                             */
/* ************************************************************************** */

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/* ************************************************************************** */
/*                            DEFINE ALL CONSTANT                             */
/* ************************************************************************** */

# define PROMPT "minishell$ "

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
	BACKU_SLASHU,
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

/* ********************************* UTILS.C ******************************** */

void				init_var(t_var *var);
char				*ft_strndup(char *line, int i, int start);
void				print_list(t_word *word);
void				count_node(t_word *word);
void				print_list_env(t_env *env);

/* ******************************** GLNODE.C ******************************** */

t_word				*get_last_tword(t_word *word);
t_env				*get_last_tenv(t_env *env);

/* ********************************* TEST.C ********************************* */

/* ********************************* ENV.C ********************************** */

void				free_list_env(t_env **env);
void				init_env(t_env **envs, char **env);
void				add_node_env(t_env **envs, char *str);

#endif