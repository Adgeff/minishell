/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_config_varlist.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 02:52:43 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/09 18:06:20 by geargenc         ###   ########.fr       */
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
	list->next = NULL;
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

int				ft_varlist_size(t_varlist *varlist)
{
	int			i;

	i = 0;
	while (varlist)
	{
		i++;
		varlist = varlist->next;
	}
	return (i);
}

char			**ft_varlist_to_tab(t_varlist *varlist)
{
	char		**tab;
	int			i;

	if (!(tab = (char **)malloc((ft_varlist_size(varlist) + 1)
		* sizeof(char *))))
		return (NULL);
	i = 0;
	while (varlist)
	{
		if (!(tab[i] = (char *)malloc((ft_strlen(varlist->name) +
			ft_strlen(varlist->value) + 2) * sizeof(char))))
			return (NULL);
		ft_strcpy(tab[i], varlist->name);
		ft_strcat(tab[i], "=");
		ft_strcat(tab[i], varlist->value);
		i++;
		varlist = varlist->next;
	}
	tab[i] = NULL;
	return (tab);
}
