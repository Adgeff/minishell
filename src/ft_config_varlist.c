/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_config_varlist.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 02:52:43 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/07 07:11:27 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_varlist		*ft_new_varlist(char *var, char *eq)
{
	t_varlist	*list;

	if (!(list = (t_varlist *)malloc(sizeof(t_varlist))) ||
		(!(list->name = ft_strsub(var, 0, eq - var))) ||
		(!(list->value = ft_strdup(eq + 1))))
		return (NULL);
	return (list);
}

int				ft_config_varlist(t_varlist **list, char **envp)
{
	char		*eq;

	if (*envp)
	{
		if ((eq = ft_strchr(*envp, '=')))
		{
			if (!(*list = ft_new_varlist(*envp, eq)))
			{
				ft_putstr_fd("ENONEM\n", 2);
				return (1);
			}
			list = &((*list)->next);
		}
		if (ft_config_varlist(list, envp + 1))
			return (1);
	}
	else
		*list = NULL;
	return (0);
}

void			ft_free_varlist(t_varlist *list)
{
	t_varlist	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

char			**ft_varlist_to_tab(t_varlist *varlist)
{
	t_varlist	*tmp;
	char		**tab;
	int			i;

	tmp = varlist;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	if (!(tab = (char **)malloc((i + 1) * sizeof(char *))))
		return (NULL);
	tmp = varlist;
	i = 0;
	while (tmp)
	{
		if (!(tab[i] = (char *)malloc((ft_strlen(tmp->name) +
			ft_strlen(tmp->value) + 2) * sizeof(char))))
			return (NULL);
		if (ft_strcpy(tab[i], tmp->name) && ft_strcat(tab[i], "=") &&
			ft_strcat(tab[i], tmp->value))
		i++;
		tmp = tmp->next;
	}
	tab[i] = NULL;
	return (tab);
}