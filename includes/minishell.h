/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 02:49:41 by geargenc          #+#    #+#             */
/*   Updated: 2018/10/26 12:36:26 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

# define BUFF_SIZE 32

typedef struct			s_varlist
{
	char				*name;
	char				*value;
	struct s_varlist	*next;
}						t_varlist;

typedef struct			s_cmdlist
{
	char				*cmd;
	struct s_cmdlist	*next;
	struct s_cmdlist	*prev;
}						t_cmdlist;

typedef struct			s_env
{
	char				*prog_name;
	t_varlist			*varlist;
	t_cmdlist			*historic;
	char				*buff;
	size_t				buff_size;
	size_t				buff_max;
	size_t				cursor;
	struct termios		init_term;
	struct termios		edit_term;
}						t_env;

/*
**						ft_config_varlist.c
*/

int						ft_new_varlist(char *var, char *eq,
						t_varlist **begin, t_varlist **cur);
t_varlist				*ft_config_varlist(char **envp);


#endif
