/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:47 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/27 11:58:08 by julmuntz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define PARENT 101
# define CHILD 202

# include "../libft/libft.h"
# include <sys/wait.h>

typedef struct s_data
{
	char			**env;
	int				file1;
	int				file2;
	char			**cmd;
	char			**args;
	char			**paths;
	int				fd[2];
	int				current_process;
}	t_data;

#endif