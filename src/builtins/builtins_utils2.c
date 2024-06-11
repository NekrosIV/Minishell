/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:20:32 by pscala            #+#    #+#             */
/*   Updated: 2024/06/11 19:26:15 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_replace_env(t_env *envp, char *str, int len, char *tmp)
{
	int	i;

	i = 0;
	if (str[0] == '$')
		i = 1;
	len = len - i;
	while (envp)
	{
		if (ft_strncmp(&str[i], envp->line, len) == 0 && (envp->line[len] == '='
				|| envp->line[len] == '\0'))
		{
			free(envp->line);
			envp->line = tmp;
			return (0);
		}
		envp = envp->next;
	}
	return (1);
}

int	replace_env(t_var *var, char *str, char *new_env_value)
{
	t_env	*envp;
	int		len;
	char	*tmp;
	char	*str_tmp;

	envp = var->env;
	len = ft_strlen(str);
	str_tmp = ft_strjoin(&str[1], "=");
	if (!str_tmp)
		return (free_error(var, E_MALLOC, "replace_env", 1), 0);
	tmp = ft_strjoin(str_tmp, new_env_value);
	if (!tmp)
		return (free(str_tmp), free_error(var, E_MALLOC, "replace_env", 1), 0);
	free(str_tmp);
	if (find_replace_env(envp, str, len, tmp) == 0)
		return (0);
	return (add_node_env(&(var->env), tmp), 0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if ((unsigned char)(s1[i]) > (unsigned char)(s2[i]))
			return (1);
		if ((unsigned char)(s1[i]) < (unsigned char)(s2[i]))
			return (-1);
		i++;
	}
	return (0);
}

int	is_builtins(t_word *tmp)
{
	if (tmp == 0 || tmp->token == REDIR_APPEND || tmp->token == REDIR_IN
		|| tmp->token == REDIR_OUT)
		return (0);
	if (ft_strcmp("cd", tmp->word) == 0)
		return (1);
	else if (ft_strcmp("pwd", tmp->word) == 0)
		return (2);
	else if (ft_strcmp("export", tmp->word) == 0)
		return (3);
	else if (ft_strcmp("unset", tmp->word) == 0)
		return (4);
	else if (ft_strcmp("echo", tmp->word) == 0)
		return (5);
	else if (ft_strcmp("env", tmp->word) == 0)
		return (6);
	else if (ft_strcmp("exit", tmp->word) == 0)
		return (7);
	return (0);
}

void	error_msg_builtins(char *builtin, char *fautife, char *err)
{
	ft_putstr_fd(builtin, 2);
	ft_putstr_fd(fautife, 2);
	if (ft_strchr(fautife, '/') != NULL || ft_strcmp(builtin, E_EXPORT) == 0)
		ft_putstr_fd(": ", 2);
	else
		ft_putstr_fd(" ", 2);
	ft_putendl_fd(err, 2);
}
