/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_config_varlist.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 02:52:43 by geargenc          #+#    #+#             */
/*   Updated: 2018/10/24 02:54:28 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_new_varlist(char *var, char *eq,
				t_varlist **begin, t_varlist **cur)
{
	if (*begin)
	{
		if (!((*cur)->next = (t_varlist *)malloc(sizeof(t_varlist))))
			return (1);
		*cur = (*cur)->next;
	}
	else
	{
		if (!(*begin = (t_varlist *)malloc(sizeof(t_varlist))))
			return (1);
		*cur = *begin;
	}
	if (!((*cur)->name = ft_strsub(var, 0, eq - var)))
		return (1);
	if (!((*cur)->value = ft_strdup(eq + 1)))
		return (1);
	return (0);
}

t_varlist		*ft_config_varlist(char **envp)
{
	t_varlist	*begin;
	t_varlist	*cur;
	char		*eq;
	int			i;

	i = 0;
	begin = NULL;
	while (envp[i])
	{
		if ((eq = ft_strchr(envp[i], '=')))
		{
			if (ft_new_varlist(envp[i], eq, &begin, &cur))
			{
				ft_putstr_fd("ENONEM\n", 2);
				return (NULL);
			}
		}
		i++;
	}
	return (begin);
}
