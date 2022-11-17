/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:15 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/17 20:22:28 by julmuntz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	check_path(char **paths, char *cmd)
{
	int		i;
	char	*file;
	char	*filepath;

	i = 0;
	while (paths[i])
	{
		file = ft_strjoin("/", cmd);
		filepath = ft_strjoin(paths[i], file);
		if (!access(filepath, F_OK))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

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
	return (NULL);
}

static int	valid_input(int arc, char *cmd1, char *cmd2, char **paths)
{
	if (arc != 5)
		return (ft_printf("Error\nWorks with 4 arguments only.\n"), FALSE);
	if (check_path(paths, cmd1) == FALSE && check_path(paths, cmd2) == TRUE)
		return (ft_printf("Error\ncannot access '%s': No such file or directory\n", cmd1), FALSE);
	else if (check_path(paths, cmd1) == TRUE && check_path(paths, cmd2) == FALSE)
		return (ft_printf("Error\ncannot access '%s': No such file or directory\n", cmd2), FALSE);
	else if (check_path(paths, cmd1) == FALSE && check_path(paths, cmd2) == FALSE)
		return (ft_printf("Error\ncannot access '%s' and '%s': No such file or directory\n", cmd1, cmd2), FALSE);
	else
		return (TRUE);
}

static char	*get_path(char **paths, char *cmd)
{
	int		i;
	char	*file;
	char	*filepath;

	i = 0;
	while (paths[i])
	{
		file = ft_strjoin("/", cmd);
		filepath = ft_strjoin(paths[i], file);
		if (!access(filepath, F_OK))
			return (filepath);
		i++;
	}
	return (NULL);
}

void	child_process(t_data *data)
{
	char	**cmd_with_flags;

	cmd_with_flags = ft_split(data->cmd1, ' ');
	if (dup2(data->file1, STDIN_FILENO) < 0)
	{
		close(STDIN_FILENO);
		data->file1 = STDIN_FILENO;
	}
	dup2(data->file1, STDIN_FILENO);
	dup2(data->portal[1], STDOUT_FILENO);
	close(data->portal[0]);
	close(data->file1);
	execve(get_path(data->paths, data->cmd1), cmd_with_flags, data->env);
}

void	pipex(t_data *data)
{
	pid_t	parent;

	pipe(data->portal);
	parent = fork();
	if (parent < 0)
		return (perror("Fork: "));
	if (!parent)
		child_process(data);
	else
		return ;
		// parent_process(data);
}

int	main(int arc, char **arv, char **env)
{
	t_data	data;

	data.env = env;
	data.paths = ft_split(find_paths(env), ':');
	data.file1 = open(arv[1], O_RDONLY);
	data.file2 = open(arv[4], O_RDWR | O_CREAT | O_TRUNC, 0644);
	data.fullcmd1 = ft_split(arv[2], ' ');
	data.fullcmd2 = ft_split(arv[3], ' ');
	if (valid_input(arc, data.fullcmd1[0], data.fullcmd2[0], data.paths) == FALSE)
		return (0);
	pipex(&data);
}
