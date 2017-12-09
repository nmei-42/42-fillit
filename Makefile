# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/27 08:57:48 by nmei              #+#    #+#              #
#    Updated: 2017/12/08 18:30:33 by nmei             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fillit
SRCS_DIR = ./
INCLUDE = ./
RM = /bin/rm -f

FILES = main fillit_read tetra_list_utils node_namelist_utils dancing_links_ops\
make_xcover_cols make_xcover_rows fillit_dlx_solve cleanup_and_print

CFILES = $(patsubst %, $(SRCS_DIR)%.c, $(FILES))
OFILES = $(patsubst %, %.o, $(FILES))

CFLAGS = -Wall -Wextra -Werror

.PHONY: all clean fclean re

all: $(NAME)

$(OFILES):
	gcc $(CFLAGS) -c -I$(INCLUDE) $(CFILES)

$(NAME): $(OFILES)
	gcc $(CFLAGS) -o $(NAME) $(OFILES)

clean:
	$(RM) $(OFILES)

fclean: clean
	$(RM) $(NAME)

re: fclean all