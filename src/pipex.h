/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:45:47 by julmuntz          #+#    #+#             */
/*   Updated: 2022/11/17 20:05:48 by julmuntz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"

typedef struct s_data
{
	char			**env;
	int				file1;
	int				file2;
	char			*cmd1;
	char			*cmd2;
	char			**paths;
	int				portal[2];
	char			**fullcmd1;
	char			**fullcmd2;
}	t_data;

#endif