/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 01:47:45 by geargenc          #+#    #+#             */
/*   Updated: 2018/11/12 17:14:11 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
