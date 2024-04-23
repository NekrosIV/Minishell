NAME = minishell
BONUS_NAME = minishell_bonus
LIBFTNAME = libft.a
CC = cc
CFLAGS = -g3 #-Wall -Wextra -Werror 
LIBFTDIR = ./libft

SRCS = ./src/main.c ./src/parsing.c ./src/utils.c ./src/get_last_node.c ./src/env.c \
	   ./src/free.c ./src/check_syntax.c ./src/befor_exe.c ./src/exec_utils.c 
BONUS_SRCS = 
OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)



HEADERS = ./include

# COLOR #
GREEN	= \033[38;5;76m
RED		= \033[38;5;160m
YELLOW	= \033[38;5;226m
ORANGE	= \033[38;5;202m
PURPLE	= \033[38;5;213m
LBLUE	= \033[38;5;51m
BLUE	= \033[38;5;117m
INDI	= \033[38;5;99m
RESET	= \033[00m

all: $(NAME)


$(NAME): $(OBJS)
	@make -C $(LIBFTDIR)
	@$(CC) $(CFLAGS) -I$(HEADERS) -o $@ $(OBJS) -L ${LIBFTDIR} -lft -lreadline
	@printf "$(BLUE)\n\n\n"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢟⣛⣭⣵⣶⣶⣬⣭⣭⣭⣝⡛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣫⣵⣾⣿⣿⣿⣿⣿⣿⣿⡿⣏⢻⣟⢿⣷⣬⡙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⣿⡿⢫⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣜⡜⣿⣆⢿⡜⣿⣎⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⠟⣴⣿⣿⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⡎⣿⢸⡟⣾⢿⡎⢻⣧⡹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⠏⣼⣿⣿⢡⣹⡟⣿⣿⣿⡿⠟⠛⠋⠉⠁⠀⠀⠉⠈⠁⠙⠘⠇⢠⣿⣧⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⠇⣼⣿⣿⣿⣾⢿⣷⠙⠋⠁⠀⠀⠀⠀⠀⢀⣀⣠⣤⣤⣴⣶⣶⣶⡶⢲⣶⣶⣬⣭⣟⣛⣿⢛⣭⣛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⡿⢸⣿⡟⣿⡆⠿⠂⠀⠀⠀⣀⣀⣤⣴⣶⣿⣿⣿⣿⣿⣿⣿⠿⠋⠁⣴⣿⣟⣖⣉⠻⠿⣿⡇⣾⣿⣿⣷⡝⢿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⡇⣾⣿⣧⢻⠏⠀⢀⣠⣴⣿⣏⣥⡾⠾⢉⣛⠛⠛⠉⠉⠁⠀⠀⠐⠛⠉⠙⠋⠋⠩⣝⣳⣾⡇⣿⣿⣿⣿⣿⢠⣝⢿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⡇⣿⣿⣿⠇⣠⡶⢋⣡⣤⠏⠉⢁⣴⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢾⣷⣦⣿⣿⠧⢻⣿⣿⣿⣿⡄⣿⣆⢻⣿⣿⣿⣿"
	@echo "⣿⣿⣿⡘⢋⣵⣾⣿⡿⠛⠉⠀⠀⠀⣼⣿⣿⣿⣿⣿⣷⡀⠀⣦⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⡛⢉⢷⡄⢻⣿⣿⡿⠇⡿⡇⠀⣿⣿⣿⣿"
	@echo "⣿⡿⢋⣴⣿⠿⠋⠉⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣷⡀⢸⣷⡀⠀⠀⠀⠀⠀⠀⠀⢀⡈⠈⢣⢿⠠⣤⠩⣶⡆⣇⣠⠀⣿⣿⣿⣿"
	@echo "⢋⡴⣫⢉⠄⠀⠀⠀⠀⠀⠀⣼⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣧⢸⠿⢛⣀⠘⣄⠀⠀⠀⠀⠈⠻⡟⢺⡼⡇⣿⡀⢙⣇⢿⡟⣼⣿⣿⣿⣿"
	@echo "⣫⡾⠛⠁⠀⣴⠀⠀⠀⠀⠀⣟⡂⡻⠿⣿⣿⣿⣿⣿⣿⣿⣋⣴⣾⠿⢿⠆⢿⡄⠀⠀⠀⠐⣦⣄⡸⢱⡇⣿⠀⠀⠹⠈⢼⣿⣿⣿⣿⣿"
	@echo "⣿⢃⢂⢊⣾⡇⠀⠀⠀⡄⢰⣿⣧⣿⣿⣶⣶⣿⣟⢻⣿⣿⣿⠋⢁⢤⡤⠂⣸⠿⠀⡆⠀⠌⠈⡿⠡⠟⣃⣭⣶⣾⣿⣷⣶⣍⡻⢿⣿⣿"
	@echo "⢁⣶⠟⢺⡏⠀⠀⠀⠀⣷⢸⡟⢋⣩⣄⣉⢹⣿⣿⣾⣿⣿⣿⣷⣯⣄⡲⠖⠩⢂⢲⡇⠉⠘⡧⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⢙⣿"
	@echo "⢸⣩⢘⢸⡇⢠⢇⣤⠀⢿⣿⢷⣿⣿⣿⣿⣿⣿⣿⣦⣉⡿⠿⠟⣛⣭⣦⣾⣷⣿⢸⡇⠸⣣⢀⠀⣦⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢸⣿"
	@echo "⣦⢻⣸⣼⡱⣿⡄⣤⠀⢸⣿⠰⠆⣍⢉⣭⣭⣭⣭⣴⣶⣶⣿⣿⣿⣿⣿⣿⢟⡁⣸⡇⠞⠁⠘⣤⡿⠿⠟⣛⣭⣭⣶⣾⣿⣿⣿⣅⣿⣿"
	@echo "⣾⣀⠿⣅⣉⢾⣧⢻⡁⠀⢿⡇⢦⣿⣿⣿⣿⣿⡿⠿⢟⣛⣻⣭⣭⣭⣭⣁⣌⢠⡿⢠⣖⡄⣄⠀⠁⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⠻"
	@echo "⠰⢭⣓⠦⣭⣥⠙⣧⣙⠦⡌⢿⡄⠙⠗⢙⣭⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⡿⣣⡞⢁⠸⠟⣽⡏⢸⢀⢰⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀"
	@echo "⣿⣶⣬⣭⣙⣛⣛⣛⣛⣓⡲⣌⠻⣦⡘⢾⣿⣿⣿⣿⣿⣿⣿⡿⠛⠛⣩⠞⠉⣠⣿⣿⣳⢷⣷⠈⠻⠾⠈⢟⣛⣛⣋⣿⣿⣿⣿⣿⣿⣦"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠈⠙⠲⠬⣭⣉⣙⣋⣠⣭⠴⠶⠋⠁⢠⣾⡿⣿⣿⣏⣿⠃⠀⠸⡇⢠⢹⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⣾⣷⣶⣶⠤⠀⠀⠶⡀⢀⠄⢠⢿⣟⣴⣿⡿⣼⠃⠀⠀⠀⠈⡉⢘⠛⣛⣫⣤⣶⣿⣿⣿⡟⣿"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠻⣟⣛⠿⣦⣤⣠⡄⠀⠌⠠⢱⡟⣼⠿⠛⣡⠁⠀⠀⠀⠀⠀⠃⢨⣇⣿⣿⣿⣿⣿⡿⢋⣾⣯"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⢀⣄⠈⢻⣿⣷⣾⣭⣛⣻⣧⣀⣛⣨⣴⣾⣿⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⠩⡙⠛⠋⠁⠈⠙⠛⠉"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⢾⣿⠇⠈⣿⣿⣿⣿⣿⣿⡇⣿⣿⣿⣿⣿⣿⡟⢻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠁⠀⠀⠈⠻⢿⣿⣿⣿⡇⣿⣿⣿⣿⡿⢋⢠⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣄"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⣀⡀⢸⣦⡒⠨⠛⠿⣷⡹⣿⣟⣫⣬⣭⣼⣿⣿⣷⣄⣀⣀⣀⣀⣀⣀⣀⣈⣭⣥⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⢘⣿⣧⣀⣻⣿⣦⣽⣿⣿⡷⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	
%.o: %.c
	@printf "$(LBLUE)[Compilation]$(RESET) In progress... $(GREEN)$<" && \
	$(CC) $(CFLAGS) -I$(HEADERS) -c $< -o $@ && \
	printf "\r$(LBLUE)[Compilation]$(RESET) Completed   ... $(GREEN)$<" && \
	printf " $(LBLUE)[$(RESET)$(CC)$(LBLUE)/]$(RESET)\n"

bonus :  ${BONUS_NAME}

${BONUS_NAME} : $(BONUS_OBJS)
	@make -C $(LIBFTDIR)
	@$(CC) $(CFLAGS) -I$(HEADERS) -o ${BONUS_NAME} $(BONUS_OBJS) -L ${LIBFTDIR} -lft
	@printf "$(BLUE)\n\n\n"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢟⣛⣭⣵⣶⣶⣬⣭⣭⣭⣝⡛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣫⣵⣾⣿⣿⣿⣿⣿⣿⣿⡿⣏⢻⣟⢿⣷⣬⡙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⣿⡿⢫⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣜⡜⣿⣆⢿⡜⣿⣎⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⠟⣴⣿⣿⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⡎⣿⢸⡟⣾⢿⡎⢻⣧⡹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⠏⣼⣿⣿⢡⣹⡟⣿⣿⣿⡿⠟⠛⠋⠉⠁⠀⠀⠉⠈⠁⠙⠘⠇⢠⣿⣧⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⠇⣼⣿⣿⣿⣾⢿⣷⠙⠋⠁⠀⠀⠀⠀⠀⢀⣀⣠⣤⣤⣴⣶⣶⣶⡶⢲⣶⣶⣬⣭⣟⣛⣿⢛⣭⣛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⡿⢸⣿⡟⣿⡆⠿⠂⠀⠀⠀⣀⣀⣤⣴⣶⣿⣿⣿⣿⣿⣿⣿⠿⠋⠁⣴⣿⣟⣖⣉⠻⠿⣿⡇⣾⣿⣿⣷⡝⢿⣿⣿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⡇⣾⣿⣧⢻⠏⠀⢀⣠⣴⣿⣏⣥⡾⠾⢉⣛⠛⠛⠉⠉⠁⠀⠀⠐⠛⠉⠙⠋⠋⠩⣝⣳⣾⡇⣿⣿⣿⣿⣿⢠⣝⢿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⡇⣿⣿⣿⠇⣠⡶⢋⣡⣤⠏⠉⢁⣴⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢾⣷⣦⣿⣿⠧⢻⣿⣿⣿⣿⡄⣿⣆⢻⣿⣿⣿⣿"
	@echo "⣿⣿⣿⡘⢋⣵⣾⣿⡿⠛⠉⠀⠀⠀⣼⣿⣿⣿⣿⣿⣷⡀⠀⣦⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⡛⢉⢷⡄⢻⣿⣿⡿⠇⡿⡇⠀⣿⣿⣿⣿"
	@echo "⣿⡿⢋⣴⣿⠿⠋⠉⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣷⡀⢸⣷⡀⠀⠀⠀⠀⠀⠀⠀⢀⡈⠈⢣⢿⠠⣤⠩⣶⡆⣇⣠⠀⣿⣿⣿⣿"
	@echo "⢋⡴⣫⢉⠄⠀⠀⠀⠀⠀⠀⣼⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣧⢸⠿⢛⣀⠘⣄⠀⠀⠀⠀⠈⠻⡟⢺⡼⡇⣿⡀⢙⣇⢿⡟⣼⣿⣿⣿⣿"
	@echo "⣫⡾⠛⠁⠀⣴⠀⠀⠀⠀⠀⣟⡂⡻⠿⣿⣿⣿⣿⣿⣿⣿⣋⣴⣾⠿⢿⠆⢿⡄⠀⠀⠀⠐⣦⣄⡸⢱⡇⣿⠀⠀⠹⠈⢼⣿⣿⣿⣿⣿"
	@echo "⣿⢃⢂⢊⣾⡇⠀⠀⠀⡄⢰⣿⣧⣿⣿⣶⣶⣿⣟⢻⣿⣿⣿⠋⢁⢤⡤⠂⣸⠿⠀⡆⠀⠌⠈⡿⠡⠟⣃⣭⣶⣾⣿⣷⣶⣍⡻⢿⣿⣿"
	@echo "⢁⣶⠟⢺⡏⠀⠀⠀⠀⣷⢸⡟⢋⣩⣄⣉⢹⣿⣿⣾⣿⣿⣿⣷⣯⣄⡲⠖⠩⢂⢲⡇⠉⠘⡧⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⢙⣿"
	@echo "⢸⣩⢘⢸⡇⢠⢇⣤⠀⢿⣿⢷⣿⣿⣿⣿⣿⣿⣿⣦⣉⡿⠿⠟⣛⣭⣦⣾⣷⣿⢸⡇⠸⣣⢀⠀⣦⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢸⣿"
	@echo "⣦⢻⣸⣼⡱⣿⡄⣤⠀⢸⣿⠰⠆⣍⢉⣭⣭⣭⣭⣴⣶⣶⣿⣿⣿⣿⣿⣿⢟⡁⣸⡇⠞⠁⠘⣤⡿⠿⠟⣛⣭⣭⣶⣾⣿⣿⣿⣅⣿⣿"
	@echo "⣾⣀⠿⣅⣉⢾⣧⢻⡁⠀⢿⡇⢦⣿⣿⣿⣿⣿⡿⠿⢟⣛⣻⣭⣭⣭⣭⣁⣌⢠⡿⢠⣖⡄⣄⠀⠁⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⠻"
	@echo "⠰⢭⣓⠦⣭⣥⠙⣧⣙⠦⡌⢿⡄⠙⠗⢙⣭⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⡿⣣⡞⢁⠸⠟⣽⡏⢸⢀⢰⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀"
	@echo "⣿⣶⣬⣭⣙⣛⣛⣛⣛⣓⡲⣌⠻⣦⡘⢾⣿⣿⣿⣿⣿⣿⣿⡿⠛⠛⣩⠞⠉⣠⣿⣿⣳⢷⣷⠈⠻⠾⠈⢟⣛⣛⣋⣿⣿⣿⣿⣿⣿⣦"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠈⠙⠲⠬⣭⣉⣙⣋⣠⣭⠴⠶⠋⠁⢠⣾⡿⣿⣿⣏⣿⠃⠀⠸⡇⢠⢹⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⣾⣷⣶⣶⠤⠀⠀⠶⡀⢀⠄⢠⢿⣟⣴⣿⡿⣼⠃⠀⠀⠀⠈⡉⢘⠛⣛⣫⣤⣶⣿⣿⣿⡟⣿"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠻⣟⣛⠿⣦⣤⣠⡄⠀⠌⠠⢱⡟⣼⠿⠛⣡⠁⠀⠀⠀⠀⠀⠃⢨⣇⣿⣿⣿⣿⣿⡿⢋⣾⣯"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⢀⣄⠈⢻⣿⣷⣾⣭⣛⣻⣧⣀⣛⣨⣴⣾⣿⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⠩⡙⠛⠋⠁⠈⠙⠛⠉"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⢾⣿⠇⠈⣿⣿⣿⣿⣿⣿⡇⣿⣿⣿⣿⣿⣿⡟⢻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠁⠀⠀⠈⠻⢿⣿⣿⣿⡇⣿⣿⣿⣿⡿⢋⢠⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣄"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⣀⡀⢸⣦⡒⠨⠛⠿⣷⡹⣿⣟⣫⣬⣭⣼⣿⣿⣷⣄⣀⣀⣀⣀⣀⣀⣀⣈⣭⣥⣿⣿⣿⣿"
	@echo "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⢘⣿⣧⣀⣻⣿⣦⣽⣿⣿⡷⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿"

clean:
	@cd $(LIBFTDIR) && make clean
	@for obj in $(OBJS); do \
		printf "$(RED)Cleaning Libft...  Removing $$obj... " && \
		sleep 0.01 && \
		rm -f $$obj && \
		printf "\r"; \
	done
	@for obj in $(BONUS_OBJS); do \
		printf "$(RED)Cleaning Libft...  Removing $$obj... " && \
		sleep 0.01 && \
		rm -f $$obj && \
		printf "\r"; \
	done

	@printf "                                                      \r$(GREEN)Minishell is Clean\n$(RESET)"

	
fclean: clean
	@cd $(LIBFTDIR) && make fclean
	@rm -f $(NAME) $(BONUS_NAME)
	
re: fclean all

.PHONY : all clean fclean re bonus
