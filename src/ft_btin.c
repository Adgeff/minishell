/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 16:55:30 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 16:56:35 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_btin				*ft_btintab(int i)
{
	static t_btin	btintab[] = {
		{"echo", &ft_echo_btin},
		{"cd", &ft_cd_btin},
		{"setenv", &ft_setenv_btin},
		{"unsetenv", &ft_unsetenv_btin},
		{"env", &ft_env_btin},
		{"exit", &ft_exit_btin},
		{"", NULL}
	};

	return (&(btintab[i]));
}

int					ft_btin(char **split)
{
	int				i;

	i = 0;
	while (ft_btintab(i)->f && ft_strcmp(ft_btintab(i)->name, split[0]))
		i++;
	if (ft_btintab(i)->f)
		return (i);
	return (-1);
}
