/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 02:49:41 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/09 19:37:23 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <limits.h>

# define BUFF_SIZE 32

typedef struct			s_varlist
{
	char				*name;
	char				*value;
	struct s_varlist	*next;
}						t_varlist;

typedef struct			s_env
{
	char				*prog_name;
	t_varlist			*varlist;
	int					exit;
	char				buff[PATH_MAX];
	int					errno;
}						t_env;

typedef struct			s_btin
{
	char				*name;
	int					(*f)(t_env *, char **argv);
}						t_btin;

/*
**						ft_config_varlist.c
*/

t_varlist				*ft_new_varlist(char *var, char *eq);
int						ft_config_varlist(t_varlist **list, char **envp);
void					ft_free_varlist(t_varlist *list);
char					**ft_varlist_to_tab(t_varlist *varlist);

#endif
