NAME = minishell
BONUS_NAME = minishell_bonus
LIBFTNAME = libft.a
CC = cc
CFLAGS = -g3 #-Wall -Wextra -Werror 
LIBFTDIR = ./libft

# Dossiers sources
SRCDIR = src
BUILTINS_DIR = $(SRCDIR)/builtins
ENV_DIR = $(SRCDIR)/env
EXEC_DIR = $(SRCDIR)/exec
EXPAND_DIR = $(SRCDIR)/expand
FREE_AND_EXIT_DIR = $(SRCDIR)/free_and_exit
MAIN_DIR = $(SRCDIR)/main
PARSING_DIR = $(SRCDIR)/parsing
UTILS_DIR = $(SRCDIR)

# Fichiers sources
SRCS = $(ENV_DIR)/env_utils.c \
      $(ENV_DIR)/env.c \
      $(EXEC_DIR)/before_exec.c \
      $(EXEC_DIR)/dup.c \
      $(EXEC_DIR)/exec_utils_two.c \
      $(EXEC_DIR)/exec_utils.c \
      $(EXEC_DIR)/exec.c \
      $(EXEC_DIR)/split_cmd.c \
      $(EXPAND_DIR)/expand_utils.c \
      $(EXPAND_DIR)/expand.c \
      $(EXPAND_DIR)/handle_expand.c \
      $(FREE_AND_EXIT_DIR)/exit.c \
      $(FREE_AND_EXIT_DIR)/free.c \
      $(MAIN_DIR)/main_utils.c \
      $(MAIN_DIR)/main.c \
      $(PARSING_DIR)/check_syntax_pipe.c \
      $(PARSING_DIR)/check_syntax_redir.c \
      $(PARSING_DIR)/check_syntax.c \
      $(PARSING_DIR)/handle_one.c \
      $(PARSING_DIR)/handle_two.c \
      $(PARSING_DIR)/parsing_utils.c \
      $(PARSING_DIR)/parsing.c \
      $(UTILS_DIR)/utils.c

OBJS = $(SRCS:.c=.o)
BONUS_SRCS = 
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
	@printf "$(YELLOW)\n\n\n"
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
	@echo "\033[00m"
	
%.o: %.c
	@printf "$(LBLUE)[Compilation]$(RESET) In progress... $(GREEN)$<" && \
	$(CC) $(CFLAGS) -I$(HEADERS) -c $< -o $@ && \
	printf "\r$(LBLUE)[Compilation]$(RESET) Completed   ... $(GREEN)$<" && \
	printf " $(LBLUE)[$(RESET)$(CC)$(LBLUE)/]$(RESET)\n"

bonus :  ${BONUS_NAME}

${BONUS_NAME} : $(BONUS_OBJS)
	@make -C $(LIBFTDIR)
	@$(CC) $(CFLAGS) -I$(HEADERS) -o ${BONUS_NAME} $(BONUS_OBJS) -L ${LIBFTDIR} -lft
	@printf "$(YELLOW)\n\n\n"
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
	@echo "\033[00m"

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