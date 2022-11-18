/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:47 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/18 02:08:21 by julmuntz         ###   ########.fr       */
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
	char			**cmd1;
	char			**cmd2;
	char			**paths;
	int				portal[2];
	int				current_process;
}	t_data;

#endif