/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:15 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/27 11:36:45 by julmuntz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*find_paths(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH", 4))
			return (env[i] + 5);
		i++;
	}
	return (0);
}

static char	*find_cmd(char *cmd, t_data *data)
{
	int		i;
	char	*file;
	char	*filepath;

	i = 0;
	data->paths = ft_split(find_paths(data->env), ':');
	while (data->paths[i])
	{
		if (!ft_strchr(cmd, '/'))
		{
			filepath = ft_strjoin(data->paths[i], "/");
			file = ft_strjoin(filepath, cmd);
			free(filepath);
			if (!access(file, F_OK))
				return (file);
			free(file);
		}
		else
			file = cmd;
		if (!access(file, F_OK))
			return (file);
		i++;
	}
	ft_free_lines(data->paths);
	return (0);
}

static int	valid_input(char *arg, t_data *data)
{
	data->cmd = ft_split(arg, ' ');
	if (!find_cmd(*data->cmd, data))
	{
		ft_free_lines(data->cmd);
		return (FALSE);
	}
	if (execve(find_cmd(*data->cmd, data), data->cmd, data->env) == -1)
	{
		ft_printf("Error\n\
Cannot access '%s': no such file or directory.\n", *data->cmd);
		return (FALSE);
	}
	return (TRUE);
}

static void	process(char *arg, t_data *data)
{
	pid_t	process_id;

	if (pipe(data->fd) < 0)
		return ;
	process_id = fork();
	if (process_id < 0)
		return ;
	if (process_id)
	{
		dup2(data->fd[0], STDIN_FILENO);
		close(data->fd[1]);
		waitpid(process_id, NULL, 0);
	}
	else
	{
		dup2(data->fd[1], STDOUT_FILENO);
		close(data->fd[0]);
		if (valid_input(arg, data) == TRUE)
			execve(find_cmd(*data->cmd, data), data->cmd, data->env);
	}
}

int	main(int arc, char **arv, char **env)
{
	int		i;
	t_data	data;

	if (arc < 5)
		return (ft_printf("Error\nWorks with at least 4 arguments.\n"), FALSE);
	data.file1 = open(arv[1], O_RDONLY);
	if (data.file1 == -1)
		return (ft_printf("Error\n\
Cannot open '%s': not accessible.\n", arv[1]), exit(EXIT_FAILURE), 0);
	data.file2 = open(arv[arc - 1], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (data.file2 == -1)
		return (ft_printf("Error\n\
Cannot open '%s': not accessible.\n", arv[arc - 1]), exit(EXIT_FAILURE), 0);
	data.env = env;
	data.args = arv;
	i = 2;
	dup2(data.file1, STDIN_FILENO);
	while (i < (arc - 2))
	{
		process(data.args[i], &data);
		i++;
	}
	dup2(data.file2, STDOUT_FILENO);
	if (valid_input(data.args[i], &data) == TRUE)
		execve(find_cmd(*data.cmd, &data), data.cmd, data.env);
}
