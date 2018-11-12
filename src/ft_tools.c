/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 17:11:42 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:18:29 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			ft_error3(char *prog_name, char *error, char *argument)
{
	ft_putstr_fd(prog_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(argument, 2);
	ft_putstr_fd("\n", 2);
}

int				ft_check_prog(t_env *env, char *prog)
{
	int			i;
	int			j;

	i = 0;
	j = 0;
	while (i < PATH_MAX - 1 && j < NAME_MAX - 1 && prog[i])
	{
		j = prog[i] == '/' ? 0 : j + 1;
		i++;
	}
	if (i == PATH_MAX - 1 || j == NAME_MAX - 1)
	{
		ft_error3(env->prog_name, "File name too long", prog);
		return (1);
	}
	if (access(prog, 0) < 0)
	{
		ft_error3(env->prog_name, "no such file or directory", prog);
		return (1);
	}
	return (0);
}

char			*ft_getenv(char *name, t_varlist *varlist)
{
	int			i;

	while (varlist)
	{
		i = 0;
		while (name[i] && name[i] != '\\' && name[i] == varlist->name[i])
			i++;
		if (!varlist->name[i] && (name[i] == '\\' || !name[i]))
			return (varlist->value);
		varlist = varlist->next;
	}
	return (NULL);
}

char			*ft_nulltoempty(char *str)
{
	static char	*empty = "";

	return (str ? str : empty);
}
