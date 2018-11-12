/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 16:57:09 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 16:57:35 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_cd(t_env *env, char *dir)
{
	char	*pwdtmp;
	char	*oldpwdtmp;

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

int			ft_cd_btin(t_env *env, char **argv)
{
	return (ft_cd(env, argv[1]));
}
