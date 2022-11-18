/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:15 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/18 23:03:15 by julmuntz         ###   ########.fr       */
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
		file = ft_strjoin("/", cmd);
		filepath = ft_strjoin(data->paths[i], file);
		free(file);
		if (!access(filepath, F_OK))
			return (free(data->paths), filepath);
		free(filepath);
		i++;
	}
	i = 0;
	while (data->paths[i])
	{
		free(data->paths[i]);
		i++;
	}
	free(data->paths);
	return (0);
}

static int	valid_input(char **arv, t_data *data)
{
	char	*cmdpath1;
	char	*cmdpath2;

	data->cmd1 = ft_split(arv[2], ' ');
	cmdpath1 = find_cmd(*data->cmd1, data);
	data->cmd2 = ft_split(arv[3], ' ');
	cmdpath2 = find_cmd(*data->cmd2, data);
	if (!cmdpath1 && cmdpath2)
		return (ft_printf("Error\n\
Cannot access '%s': no such file or directory.\n", *data->cmd1),
			free(data->cmd1), FALSE);
	else if (cmdpath1 && !cmdpath2)
		return (ft_printf("Error\n\
Cannot access '%s': no such file or directory.\n", *data->cmd2),
			free(data->cmd2), FALSE);
	else if (!cmdpath1 && !cmdpath2)
		return (ft_printf("Error\n\
Cannot access '%s' and '%s': no such files or directories.\n",
				*data->cmd1, *data->cmd2), free(data->cmd1),
			free(data->cmd2), FALSE);
	else
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
		execve(find_cmd(*data->cmd1, data), data->cmd1, data->env);
	}
	else if (data->current_process == PARENT)
	{
		dup2(data->file2, STDOUT_FILENO);
		dup2(data->portal[0], STDIN_FILENO);
		close(data->portal[0]);
		close(data->portal[1]);
		execve(find_cmd(*data->cmd2, data), data->cmd2, data->env);
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
	if (valid_input(arv, &data) == FALSE)
		return (exit(EXIT_FAILURE), 0);
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
