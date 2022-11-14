# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: julmuntz <julmuntz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/14 09:39:23 by julmuntz          #+#    #+#              #
#    Updated: 2022/11/14 09:50:01 by julmuntz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCDIR		= 	src/
SRC			=	pipex.c				\

# B_SRCDIR	= 	bonus/
# B_SRC		=	pipex_bonus.c		\

OBJDIR		= 	obj/
OBJ			= 	$(addprefix $(OBJDIR), $(SRC:.c=.o))

# B_OBJDIR	= 	$(OBJDIR)
# B_OBJ		= 	$(addprefix $(B_OBJDIR), $(B_SRC:.c=.o))

NAME		= 	pipex
# B_NAME		= 	pipex

LIBFT		=	./libft

CC			= 	cc
RM			= 	rm -f
CFLAGS		= 	-Wall -Wextra -Werror -g

$(OBJDIR)%.o:	$(SRCDIR)%.c
					@mkdir -p $(OBJDIR)
					$(CC) $(CFLAGS) -c $< -o $@

# $(B_OBJDIR)%.o:	$(B_SRCDIR)%.c
# 					@mkdir -p $(B_OBJDIR)
# 					$(CC) $(CFLAGS) -c $< -o $@

$(NAME):		$(OBJ) libft/libft.a
					@$(MAKE) --no-print-directory -C $(LIBFT)
					$(CC) -g $^ -o $@

# $(B_NAME):		$(B_OBJ) libft/libft.a
# 					@$(MAKE) --no-print-directory -C $(LIBFT)
# 					$(CC) -g $^ -o $@

all:			$(NAME) # $(B_NAME)

# bonus:			$(B_NAME)

libft/libft.a:
					@$(MAKE) --no-print-directory -C $(LIBFT) libft.a

clean:
					$(RM) -r $(OBJDIR)
					@$(RM) -r $(B_OBJDIR)
					$(RM) -r libft/obj/

fclean:			clean
					$(RM) $(NAME)
#					$(RM) $(B_NAME)
					$(RM) libft/libft.a

re:				fclean all

.PHONY:			all bonus clean fclean re