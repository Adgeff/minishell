/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 04:45:11 by geargenc          #+#    #+#             */
/*   Updated: 2018/10/26 21:44:23 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

	// struct termios	termios_s;

	// tcgetattr(0, &termios_s);
	// termios_s.c_lflag = termios_s.c_lflag & ~ISIG;
	// tcsetattr(0, 0, &termios_s);

	// pid_t	pid;

	// if (argc > 1)
	// {
	// 	pid = fork();
	// 	if (pid == 0)
	// 	{
	// 		execve(argv[1], argv + 1, envp);
	// 		exit(EXIT_SUCCESS);
	// 	}
	// 	wait(&pid);
	// }

/*int			main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	struct termios	termios_s;
	struct termios	cpy_s;
	int				buf;
	size_t			ret;

	tcgetattr(0, &termios_s);
	tcgetattr(0, &cpy_s);
	termios_s.c_lflag = termios_s.c_lflag & ~ISIG;
	termios_s.c_lflag = termios_s.c_lflag & ~ICANON;
	termios_s.c_lflag = termios_s.c_lflag & ~ECHO;
	tcsetattr(0, 0, &termios_s);
	buf = 0;
	ret = read(0, &buf, 4);
	printf("%x\n", buf);
	// if (argc > 1)
	// {

	// }
	tcsetattr(0, 0, &cpy_s);
	return (0);
}*/

int					ft_put(int c)
{
	ft_putchar_fd(c, 1);
	return (0);
}

void				ft_putcursor(size_t spaces)
{
	char			*tistr;

	if (tigetflag("cub1"))
	{
		tistr = tigetstr("cub1");
		while (spaces)
		{
			tputs(tistr, 1, &ft_put);
			spaces--;
		}
	}
}

void				ft_del(t_env *env)
{
	if (env->cursor)
	{
		ft_memmove(env->buff + env->cursor - 1, env->buff + env->cursor,
			env->buff_size - env->cursor);
		env->cursor--;
		env->buff_size--;
		if (tigetflag("cub1"))
			tputs(tigetstr("cub1"), 1, &ft_put);
		write(0, env->buff + env->cursor, env->buff_size - env->cursor);
		write(0, " ", 1);
		ft_putcursor(env->buff_size - env->cursor + 1);
	}
}

void				ft_leftarrow(t_env *env)
{
	if (env->cursor)
	{
		env->cursor--;
		if (tigetflag("cub1"))
		{
			tputs(tigetstr("cub1"), 1, &ft_put);
		}
	}
}

void				ft_rightarrow(t_env *env)
{
	if (env->cursor < env->buff_size)
	{
		write(0, env->buff + env->cursor, 1);
		env->cursor++;
	}
}

int					ft_addbuff(t_env *env, char *buff, ssize_t size)
{
	char			*new;

	if (env->buff_size + size > env->buff_max)
	{
		env->buff_max = env->buff_max + BUFF_SIZE;
		if (!(new = (char *)malloc(env->buff_max * sizeof(char))))
			return (1);
		ft_memcpy(new, env->buff, env->cursor);
	}
	else
		new = env->buff;
	ft_memmove(new + env->cursor + size, env->buff + env->cursor,
		env->buff_size - env->cursor);
	ft_memcpy(new + env->cursor, buff, size);
	if (env->buff != new)
	{
		free(env->buff);
		env->buff = new;
	}
	env->buff_size = env->buff_size + size;
	write(0, env->buff + env->cursor, env->buff_size - env->cursor);
	env->cursor = env->cursor + size;
	ft_putcursor(env->buff_size - env->cursor);
	return (0);
}

char				*ft_strndup(char *str, size_t n)
{
	char			*dup;
	size_t			i;

	i = 0;
	while (i < n && str[i])
		i++;
	if (!(dup = (char *)malloc((i + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	while (i < n && str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
void				ft_putspaces(int spaces)
{
	while (spaces--)
		write(0, " ", 1);
}

int					ft_prompt(t_env *env)
{
	unsigned int	buff;
	ssize_t			ret;
	t_cmdlist		*new;

	env->historic = NULL;
	while (1)
	{
		ft_putstr_fd("$> ", 0);
		env->buff_size = 0;
		env->cursor = 0;
		if (!(new = (t_cmdlist *)malloc(sizeof(t_cmdlist))))
			return (1);
		if (env->historic)
			env->historic->next = new;
		new->prev = env->historic;
		env->historic = new;
		new->cmd = NULL;
		while (1)
		{
			buff = 0;
			ret = read(0, &buff, 4);
			if (buff == '\n')
			{
				write(0, "\n", 1);
				break;
			}
			else if (buff == '~')
				exit(EXIT_SUCCESS);
			else if (buff == 127)
				ft_del(env);
			else if (buff == 4479771)
				ft_leftarrow(env);
			else if (buff == 4414235)
				ft_rightarrow(env);
			else if (buff == 4283163)
			{
				if (!env->historic->next)
				{
					if (!env->historic->cmd)
						free(env->historic->cmd);
					if (!(new->cmd = ft_strndup(env->buff, env->buff_size)))
					return (1);
				}
				if (env->historic->prev)
				{
					env->historic = env->historic->prev;
					write(0, env->buff + env->cursor,
						env->buff_size - env->cursor);
					ft_putcursor(env->cursor);
					ft_putspaces(env->cursor);
					ft_putcursor(env->cursor);
					ft_strcpy(env->buff, env->historic->cmd);
					env->buff_size = ft_strlen(env->historic->cmd);
					env->cursor = env->buff_size;
					write(0, env->buff, env->buff_size);
				}
			}
			else if (buff == '`')
			{
				buff = 0;
				read(0, &buff, 4);
				printf("%u\n", buff);
			}
			else
			{
				if (ft_addbuff(env, (char *)&buff, ret))
					return (1);
			}
		}
		if (new->cmd)
			free(new->cmd);
		if (!(new->cmd = ft_strndup(env->buff, env->buff_size)))
			return (1);
	}
}

int				ft_config_buff(t_env *env)
{
	if (!(env->buff = (char *)malloc(BUFF_SIZE * sizeof(char))))
	{
		ft_putstr_fd("ENONEM\n", 2);
		return (1);
	}
	env->buff_max = BUFF_SIZE;
	env->buff_size = 0;
	env->cursor = 0;
	return (0);
}

void			ft_config_termios(t_env *env)
{
	tcgetattr(0, &(env->init_term));
	tcgetattr(0, &(env->edit_term));
	env->edit_term.c_lflag = env->edit_term.c_lflag & ~ICANON;
	env->edit_term.c_lflag = env->edit_term.c_lflag & ~ECHO;
	tcsetattr(0, 0, &(env->edit_term));
	setupterm(NULL, 1, NULL);
}

int				main(int argc, char **argv, char **envp)
{
	t_env		env;

	env.prog_name = argv[0];
	if (!(env.varlist = ft_config_varlist(envp)))
		return (1);
	(void)argv;
	if (argc > 1)
	{

	}
	else
	{
		if (ft_config_buff(&env))
			return (1);
		ft_config_termios(&env);
		if (ft_prompt(&env))
			return (1);
	}
	return (0);
}
