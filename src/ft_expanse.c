/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expanse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 17:07:08 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:17:02 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*ft_varexpanse2(t_env *env, char *arg, size_t size)
{
	char	*var;
	char	*res;
	int		i;
	int		j;

	if (!(res = (char *)malloc((size + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			var = ft_nulltoempty(ft_getenv(arg + i + 1, env->varlist));
			strcpy(res + j, var);
			j = j + ft_strlen(var);
			while (arg[i] && (i == 0 || arg[i - 1] != '\\'))
				i++;
		}
		while (arg[i] && arg[i] != '$')
			res[j++] = arg[i++];
	}
	res[j] = '\0';
	return (res);
}

char		*ft_varexpanse(t_env *env, char *arg)
{
	int		i;
	size_t	size;

	i = 0;
	size = 0;
	if (!ft_strchr(arg, '$'))
		return (arg);
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			size = size + ft_strlen(ft_nulltoempty(ft_getenv(arg + i + 1,
				env->varlist)));
			while (arg[i] && (i == 0 || arg[i - 1] != '\\'))
				i++;
		}
		while (arg[i] && arg[i] != '$')
		{
			i++;
			size++;
		}
	}
	return (ft_varexpanse2(env, arg, size));
}

char		*ft_homeexpanse(t_env *env, char *arg)
{
	char	*res;
	char	*var;

	if (arg[0] == '~')
	{
		var = ft_nulltoempty(ft_getenv("HOME", env->varlist));
		if (!(res = (char *)malloc((ft_strlen(var) + ft_strlen(arg + 1) + 1)
			* sizeof(char))))
			return (NULL);
		ft_strcpy(res, var);
		ft_strcat(res, arg + 1);
	}
	else
		return (arg);
	return (res);
}

int			ft_expanse(t_env *env, char **split)
{
	char	*tmp;
	int		i;

	i = 0;
	while (split[i])
	{
		if (!(tmp = ft_varexpanse(env, split[i])))
			return (1);
		else if (tmp != split[i])
		{
			free(split[i]);
			split[i] = tmp;
		}
		if (!(tmp = ft_homeexpanse(env, split[i])))
			return (1);
		else if (tmp != split[i])
		{
			free(split[i]);
			split[i] = tmp;
		}
		i++;
	}
	return (0);
}
