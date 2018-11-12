/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 02:49:41 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:17:30 by geargenc         ###   ########.fr       */
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
**						ft_btin.c
*/

t_btin					*ft_btintab(int i);
int						ft_btin(char **split);

/*
**						ft_cd.c
*/

int						ft_cd(t_env *env, char *dir);
int						ft_cd_btin(t_env *env, char **argv);

/*
**						ft_config_varlist.c
*/

t_varlist				*ft_new_varlist(char *var, char *eq);
int						ft_config_varlist(t_varlist **list, char **envp);
void					ft_free_varlist(t_varlist *list);
char					**ft_varlist_to_tab(t_varlist *varlist);

/*
**						ft_echo.c
*/

int						ft_echo_btin(t_env *env, char **argv);

/*
**						ft_env.c
*/

int						ft_env_btin(t_env *env, char **argv);

/*
**						ft_exit.c
*/

int						ft_exit_btin(t_env *env, char **argv);

/*
**						ft_expanse.c
*/

char					*ft_varexpanse2(t_env *env, char *arg, size_t size);
char					*ft_varexpanse(t_env *env, char *arg);
char					*ft_homeexpanse(t_env *env, char *arg);
int						ft_expanse(t_env *env, char **split);

/*
**						ft_input.c
*/

int						ft_exe(t_env *env, char *prog, char **split);
int						ft_cmd(t_env *env, char **split);
int						ft_input(t_env *env, char *input);

/*
**						ft_path.c
*/

int						ft_get_path(t_env *env, char *path, int i,
						char **split);
int						ft_explore_path(t_env *env, char **split);

/*
**						ft_setenv.c
*/

int						ft_setenv(char *var, t_varlist **list);
int						ft_setenv_btin(t_env *env, char **argv);

/*
**						ft_tools.c
*/

void					ft_error3(char *prog_name, char *error, char *argument);
int						ft_check_prog(t_env *env, char *prog);
char					*ft_getenv(char *name, t_varlist *varlist);
char					*ft_nulltoempty(char *str);

/*
**						ft_unsetenv.c
*/

int						ft_unsetenv_btin(t_env *env, char **argv);

#endif
