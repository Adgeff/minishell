/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 17:09:05 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:09:22 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_get_path(t_env *env, char *path, int i, char **split)
{
	int		j;
	int		k;

	j = 0;
	k = 0;
	while (j < PATH_MAX - 1 && k < NAME_MAX - 1 && path[i + j]
		&& path[i + j] != ':')
	{
		env->buff[j] = path[i + j];
		k = path[i + j] == '/' ? 0 : k + 1;
		j++;
	}
	if (k == NAME_MAX - 1)
		return (-1);
	env->buff[j] = '/';
	k = 0;
	while (j + k < PATH_MAX - 2 && k < NAME_MAX - 2 && split[0][k])
	{
		env->buff[j + k + 1] = split[0][k];
		k++;
	}
	if (j + k == PATH_MAX - 2 || k == NAME_MAX - 2)
		return (-1);
	env->buff[j + k + 1] = '\0';
	return (j);
}

int			ft_explore_path(t_env *env, char **split)
{
	char	*path;
	int		i;
	int		ret;

	path = ft_nulltoempty(ft_getenv("PATH", env->varlist));
	i = 0;
	while (path[i])
	{
		while (path[i] == ':')
			i++;
		if ((ret = ft_get_path(env, path, i, split)) < 0)
		{
			ft_error3(env->prog_name, "File name too long", split[0]);
			return (1);
		}
		i = i + ret;
		if (!access(env->buff, 0))
			return (0);
	}
	ft_error3(env->prog_name, "command not found", split[0]);
	return (1);
}
