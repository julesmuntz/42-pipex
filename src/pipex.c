/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:15 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/16 22:22:28 by julmuntz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// void	child_process(int f1, int *portal, char *cmd1)
// {
// 	if (dup2(f1, STDIN_FILENO) < 0)
// 	{
// 		close(STDIN_FILENO);
// 		f1 = STDIN_FILENO;
// 	}
// 	dup2(f1, STDIN_FILENO);
// 	f1 = execve("/usr/bin/", cmd1, NULL);
// 	dup2(portal[1], STDOUT_FILENO);
// 	portal[1] = execve("/usr/bin/", cmd1, NULL);
// }

// void	pipex(int f1, int f2, char **arv, char **env)
// {
// 	int		portal[2];
// 	pid_t	parent;

// 	pipe(portal);
// 	parent = fork();
// 	if (parent < 0)
// 		return (perror("Fork: "));
// 	if (!parent)
// 		child_process(f1, portal, arv[2]);
// 	else
// 		parent_process(f2, portal, arv[3]);
// }

int	find_cmd(char **paths, char *cmd1, char *cmd2)
{
	int		i;
	int		cmd1_found;
	int		cmd2_found;

	i = 0;
	cmd1_found = FALSE;
	cmd2_found = FALSE;
	while (paths[i])
	{
		cmd1 = ft_strjoin(paths[i], cmd1);
		cmd2 = ft_strjoin(paths[i], cmd2);
		if (cmd1_found == TRUE && cmd2_found == TRUE)
			return (TRUE);
		if (cmd1_found == FALSE && !access(cmd1, F_OK))
			cmd1_found = TRUE;
		if (cmd2_found == FALSE && !access(cmd2, F_OK))
			cmd2_found = TRUE;
		i++;
	}
	return (FALSE);
}

char	*find_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH", 4))
			return (env[i]);
		i++;
	}
	return (NULL);
}

int	main(int arc, char **arv, char **env)
{
	(void)	arc;
	char	**paths;

	// if (arc != 5)
	// 	return (ft_printf("Error\n"), 0);
	paths = ft_split(find_path(env), ':');
	*paths += 5;
	if (find_cmd(paths, arv[2], arv[3]) == TRUE)
		return (ft_printf("Found\n"), 0);
	else
		return (ft_printf("Error\n"), 0);
	return (0);
}
