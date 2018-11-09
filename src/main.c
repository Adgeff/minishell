/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 01:47:45 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/09 20:13:33 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_error3(char *prog_name, char *error, char *argument)
{
	ft_putstr_fd(prog_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(argument, 2);
	ft_putstr_fd("\n", 2);
}

int			ft_check_prog(t_env *env, char *prog)
{
	int		i;
	int		j;

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

int			ft_exe(t_env *env, char *prog, char **split)
{
	char	**envp;
	pid_t	pid;
	int		i;

	if (!(envp = ft_varlist_to_tab(env->varlist)))
		return (1);
	if (!(pid = fork()))
	{
		if (execve(prog, split, envp) == -1)
			ft_error3(env->prog_name, "permission denied", split[0]);
		exit(EXIT_SUCCESS);
	}
	else
		waitpid(pid, NULL, 0);
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	return (0);
}

int					ft_echo_btin(t_env *env, char **argv)
{
	int				i;

	(void)env;
	i = 1;
	while (argv[i])
	{
		ft_putstr(argv[i]);
		i++;
		if (argv[i])
			ft_putstr(" ");
	}
	ft_putstr("\n");
	return (0);
}

int					ft_env_btin(t_env *env, char **argv);

int					ft_setenv(char *var, t_varlist **list)
{
	char			*chr;

	if (!(chr = ft_strchr(var, '=')))
		return (0);
	while (*list && ft_strncmp(var, (*list)->name, chr - var))
		list = &((*list)->next);
	if (*list)
	{
		free((*list)->value);
		if (!((*list)->value = ft_strdup(chr + 1)))
			return (1);
	}
	else if (!(*list = ft_new_varlist(var, chr)))
		return (1);
	return (0);
}

int					ft_setenv_btin(t_env *env, char **argv)
{
	if (argv[1] == NULL)
		return (ft_env_btin(env, argv));
	return (ft_setenv(argv[1], &(env->varlist)));
}

int					ft_unsetenv_btin(t_env *env, char **argv)
{
	t_varlist		*tmp;
	t_varlist		**list;

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

int					ft_env_btin(t_env *env, char **argv)
{
	t_varlist		*list;

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

int					ft_exit_btin(t_env *env, char **argv)
{
	(void)argv;
	env->exit = 1;
	return (0);
}

char		*ft_getenv(char *name, t_varlist *varlist);

int					ft_cd(t_env *env, char *dir)
{
	char			*pwdtmp;
	char			*oldpwdtmp;

	if (!dir)
		return (ft_cd(env, ft_getenv("HOME", env->varlist)));
	else if (!ft_strcmp(dir, "-"))
		return (ft_cd(env, ft_getenv("OLDPWD", env->varlist)));
	else if (access(dir, 0))
		ft_error3(env->prog_name, "no such file or directory", dir);
	else if (chdir(dir) < 0)
		ft_error3(env->prog_name, "permission denied", dir);
	else
	{
		if (!(pwdtmp = getcwd(NULL, 0)) ||
			!(pwdtmp = ft_strjoinfree("PWD=", pwdtmp, 2)) ||
			!(oldpwdtmp = ft_strjoin("OLDPWD=",
			ft_getenv("PWD", env->varlist))) ||
			ft_setenv(pwdtmp, &(env->varlist)) ||
			ft_setenv(oldpwdtmp, &(env->varlist)))
			return (1);
		free(pwdtmp);
		free(oldpwdtmp);
	}
	return (0);
}

int					ft_cd_btin(t_env *env, char **argv)
{
	return (ft_cd(env, argv[1]));
}

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

int			ft_btin(char **split)
{
	int		i;

	i = 0;
	while (ft_btintab(i)->f && ft_strcmp(ft_btintab(i)->name, split[0]))
		i++;
	if (ft_btintab(i)->f)
		return (i);
	return (-1);
}

char		*ft_getenv(char *name, t_varlist *varlist)
{
	int		i;

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

int			ft_get_path(t_env *env, char *path, int i, char **split)
{
	int		j;
	int		k;

	j = 0;
	k = 0;
	while (j < PATH_MAX - 1 && k < NAME_MAX - 1 && path[i + j]
		&& path[i + j] != ':')
	{
		env->buff[j] = path[i + j];
		k = path[i + j] == '/' ? 0 : k + 1;
		j++;
	}
	if (k == NAME_MAX - 1)
		return (-1);
	env->buff[j] = '/';
	k = 0;
	while (j + k < PATH_MAX - 2 && k < NAME_MAX - 2 && split[0][k])
	{
		env->buff[j + k + 1] = split[0][k];
		k++;
	}
	if (j + k == PATH_MAX - 2 || k == NAME_MAX - 2)
		return (-1);
	env->buff[j + k + 1] = '\0';
	return (j);
}

int			ft_explore_path(t_env *env, char **split)
{
	char	*path;
	int		i;
	int		ret;

	if (!(path = ft_getenv("PATH", env->varlist)))
		return (1);
	i = 0;
	while (path[i])
	{
		while (path[i] == ':')
			i++;
		if ((ret = ft_get_path(env, path, i, split)) < 0)
		{
			ft_error3(env->prog_name, "File name too long", split[0]);
			return (1);
		}
		i = i + ret;
		if (!access(env->buff, 0))
			return (0);
	}
	ft_error3(env->prog_name, "command not found", split[0]);
	return (1);
}

int			ft_cmd(t_env *env, char **split)
{
	int		ret;

	if ((ret = ft_btin(split)) < 0)
	{
		if (!ft_explore_path(env, split) && ft_exe(env, env->buff, split))
			return (1);
	}
	else if (ft_btintab(ret)->f(env, split))
		return (1);
	return (0);
}

char			*ft_nulltoempty(char *str)
{
	static char	*empty = "";

	return (str ? str : empty);
}

char		*ft_varexpanse2(t_env *env, char *arg, size_t size)
{
	char	*var;
	char	*res;
	int		i;
	int		j;

	if (!(res = (char *)malloc((size + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			var = ft_nulltoempty(ft_getenv(arg + i + 1, env->varlist));
			strcpy(res + j, var);
			j = j + ft_strlen(var);
			while (arg[i] && (i == 0 || arg[i - 1] != '\\'))
				i++;
		}
		while (arg[i] && arg[i] != '$')
			res[j++] = arg[i++];
	}
	res[j] = '\0';
	return (res);
}

char		*ft_varexpanse(t_env *env, char *arg)
{
	int		i;
	size_t	size;

	i = 0;
	size = 0;
	if (!ft_strchr(arg, '$'))
		return (arg);
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			size = size + ft_strlen(ft_nulltoempty(ft_getenv(arg + i + 1,
				env->varlist)));
			while (arg[i] && (i == 0 || arg[i - 1] != '\\'))
				i++;
		}
		while (arg[i] && arg[i] != '$')
		{
			i++;
			size++;
		}
	}
	return (ft_varexpanse2(env, arg, size));
}

char		*ft_homeexpanse(t_env *env, char *arg)
{
	char	*res;
	char	*var;

	if (arg[0] == '~')
	{
		var = ft_nulltoempty(ft_getenv("HOME", env->varlist));
		if (!(res = (char *)malloc((ft_strlen(var) + ft_strlen(arg + 1) + 1)
			* sizeof(char))))
			return (NULL);
		ft_strcpy(res, var);
		ft_strcat(res, arg + 1);
	}
	else
		return (arg);
	return (res);
}

int			ft_expanse(t_env *env, char **split)
{
	char	*tmp;
	int		i;

	i = 0;
	while (split[i])
	{
		if (!(tmp = ft_varexpanse(env, split[i])))
			return (1);
		else if (tmp != split[i])
		{
			free(split[i]);
			split[i] = tmp;
		}
		if (!(tmp = ft_homeexpanse(env, split[i])))
			return (1);
		else if (tmp != split[i])
		{
			free(split[i]);
			split[i] = tmp;
		}
		i++;
	}
	return (0);
}

int			ft_input(t_env *env, char *input)
{
	char	**split;
	int		i;

	if (!(split = ft_strsplitwhitespaces(input)) ||
		ft_expanse(env, split))
		return (1);
	if (split[0])
	{
		if (ft_strchr(split[0], '/') ? !ft_check_prog(env, split[0]) &&
			ft_exe(env, split[0], split) : ft_cmd(env, split))
			return (1);
	}
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (0);
}

int			ft_minishell(t_env *env)
{
	char	*input;

	env->exit = 0;
	while (!env->exit)
	{
		ft_putstr_fd("$> ", 1);
		if (get_next_line(0, &input) <= 0 ||
			ft_input(env, input))
			return (1);
		free(input);
	}
	return (0);
}

int			main(int argc, char **argv, char **envp)
{
	t_env	env;

	if (argc > 0)
		env.prog_name = argv[0];
	else
		env.prog_name = "minishell";
	if (ft_config_varlist(&(env.varlist), envp))
		return (0);
	if (ft_minishell(&env))
		return (0);
	ft_free_varlist(env.varlist);
	return (0);
}
