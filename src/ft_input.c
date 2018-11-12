/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 17:14:18 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:15:14 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
