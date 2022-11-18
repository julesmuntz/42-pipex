/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:15 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/18 02:16:55 by julmuntz         ###   ########.fr       */
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
	while (data->paths[i])
	{
		file = ft_strjoin("/", cmd);
		filepath = ft_strjoin(data->paths[i], file);
		free(file);
		if (!access(filepath, F_OK))
			return (filepath);
		free(filepath);
		i++;
	}
	return (0);
}

static int	valid_input(int arc, t_data *data)
{
	if (arc != 5)
		return (ft_printf("Error\nWorks with 4 arguments.\n"), FALSE);
	if (!find_cmd(*data->cmd1, data)
		&& find_cmd(*data->cmd2, data))
		return (ft_printf("Error\n\
Cannot access '%s': no such file or directory.\n", *data->cmd1), FALSE);
	else if (find_cmd(*data->cmd1, data)
		&& !find_cmd(*data->cmd2, data))
		return (ft_printf("Error\n\
Cannot access '%s': no such file or directory.\n", *data->cmd2), FALSE);
	else if (!find_cmd(*data->cmd1, data)
		&& !find_cmd(*data->cmd2, data))
		return (ft_printf("Error\n\
Cannot access '%s' and '%s': no such files or directories.\n",
				*data->cmd1, *data->cmd2), FALSE);
	else
		return (TRUE);
}

void	process(t_data *data)
{
	if (data->current_process == CHILD)
	{
		if (data->file1 < 0)
			return ;
		dup2(data->file1, STDIN_FILENO);
		dup2(data->portal[1], STDOUT_FILENO);
		close(data->portal[0]);
		execve(find_cmd(*data->cmd1, data), data->cmd1, data->env);
	}
	else if (data->current_process == PARENT)
	{
		if (data->file2 < 0)
			return ;
		dup2(data->file2, STDOUT_FILENO);
		dup2(data->portal[0], STDIN_FILENO);
		close(data->portal[1]);
		execve(find_cmd(*data->cmd2, data), data->cmd2, data->env);
	}
}

int	main(int arc, char **arv, char **env)
{
	t_data	data;
	pid_t	process_id;

	data.env = env;
	data.paths = ft_split(find_paths(env), ':');
	data.cmd1 = ft_split(arv[2], ' ');
	data.cmd2 = ft_split(arv[3], ' ');
	if (valid_input(arc, &data) == FALSE)
		return (0);
	data.file1 = open(arv[1], O_RDONLY);
	data.file2 = open(arv[4], O_RDWR | O_CREAT | O_TRUNC, 0644);
	pipe(data.portal);
	process_id = fork();
	if (process_id < 0)
		return (0);
	if (!process_id)
	{
		data.current_process = CHILD;
		process(&data);
	}
	data.current_process = PARENT;
	waitpid(process_id, NULL, 0);
	process(&data);
}
