# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mawad <mawad@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/12 00:08:55 by mawad             #+#    #+#              #
#    Updated: 2024/04/13 22:03:11 by mawad            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
BONUS_NAME = philo_bonus

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

PHILO_DIR = Srcs
PHILO_SRCS = $(addprefix $(PHILO_DIR)/, main.c philo_utils.c philo_utils2.c handlers.c inits.c simulation.c actions_utils.c actions.c read_write.c monitor.c parser.c)

BONUS_DIR = Bonus
BONUS_SRCS = $(addprefix $(BONUS_DIR)/, actions_bonus.c ft_itoa_bonus.c handlers_bonus.c handlers_bonus2.c inits_bonus.c main_bonus.c monitor_bonus.c \
										 parser_bonus.c philo_utils_bonus.c philo_utils_bonus2.c philo_utils_bonus3.c read_write_bonus.c simulation_bonus.c)

PHILO_OBJS = $(PHILO_SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(PHILO_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(PHILO_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
bonus: $(BONUS_NAME)
	
$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $(BONUS_OBJS)

$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PHILO_OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

rebonus: fclean bonus

.PHONY: all clean fclean re