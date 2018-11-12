/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 17:03:07 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:04:17 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_setenv(char *var, t_varlist **list)
{
	char	*chr;

	if (!(chr = ft_strchr(var, '=')))
		return (0);
	while (*list && ft_strncmp(var, (*list)->name, chr - var))
		list = &((*list)->next);
	if (*list)
	{
		free((*list)->value);
		if (!((*list)->value = ft_strdup(chr + 1)))
			return (1);
	}
	else if (!(*list = ft_new_varlist(var, chr)))
		return (1);
	return (0);
}

int			ft_setenv_btin(t_env *env, char **argv)
{
	if (argv[1] == NULL)
		return (ft_env_btin(env, argv));
	return (ft_setenv(argv[1], &(env->varlist)));
}
