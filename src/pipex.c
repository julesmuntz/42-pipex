/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:15 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/21 16:31:32 by julmuntz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (!find_cmd(data->cmd[0], data))
	{
		ft_free_lines(data->cmd);
		return (FALSE);
	}
	if (execve(find_cmd(data->cmd[0], data), data->cmd, data->env) == -1)
	{
		ft_printf("Error\n\
Cannot access '%s': no such file or directory.\n", data->cmd[0]);
		return (FALSE);
	}
	return (TRUE);
}

static void	process(t_data *data)
{
	if (data->current_process == CHILD)
	{
		dup2(data->file1, STDIN_FILENO);
		dup2(data->portal[1], STDOUT_FILENO);
		close(data->portal[1]);
		close(data->portal[0]);
		if (valid_input(data->args[2], data) == TRUE)
		{
			data->cmd1 = data->cmd;
			execve(find_cmd(data->cmd1[0], data), data->cmd1, data->env);
		}
	}
	else if (data->current_process == PARENT)
	{
		dup2(data->file2, STDOUT_FILENO);
		dup2(data->portal[0], STDIN_FILENO);
		close(data->portal[0]);
		close(data->portal[1]);
		if (valid_input(data->args[3], data) == TRUE)
		{
			data->cmd2 = data->cmd;
			execve(find_cmd(data->cmd2[0], data), data->cmd2, data->env);
		}
	}
}

int	main(int arc, char **arv, char **env)
{
	t_data	data;
	pid_t	process_id;

	if (arc != 5)
		return (ft_printf("Error\nWorks with 4 arguments.\n"), FALSE);
	data.file1 = open(arv[1], O_RDONLY);
	if (data.file1 == -1)
		return (ft_printf("Error\n\
Cannot open '%s': not accessible.\n", arv[1]), exit(EXIT_FAILURE), 0);
	data.file2 = open(arv[4], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (data.file2 == -1)
		return (ft_printf("Error\n\
Cannot open '%s': not accessible.\n", arv[4]), exit(EXIT_FAILURE), 0);
	data.env = env;
	data.args = arv;
	pipe(data.portal);
	process_id = fork();
	if (process_id < 0)
		return (0);
	if (process_id)
	{
		data.current_process = PARENT;
		process(&data);
	}
	data.current_process = CHILD;
	process(&data);
}
