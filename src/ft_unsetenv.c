/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsetenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 17:01:31 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:01:47 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_unsetenv_btin(t_env *env, char **argv)
{
	t_varlist	*tmp;
	t_varlist	**list;

	if (!argv[1])
		return (0);
	list = &(env->varlist);
	while (*list && ft_strcmp((*list)->name, argv[1]))
		list = &((*list)->next);
	if (*list)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
	return (0);
}
