/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 16:59:54 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:00:45 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_env_btin(t_env *env, char **argv)
{
	t_varlist	*list;

	(void)argv;
	list = env->varlist;
	while (list)
	{
		ft_putstr(list->name);
		ft_putstr("=");
		ft_putstr(list->value);
		ft_putstr("\n");
		list = list->next;
	}
	return (0);
}
