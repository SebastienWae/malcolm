SHELL := /bin/bash

RM := rm -rf
MKDIR := mkdir -p
LESS := less
GDB := gdb
VALGRIND := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRIND_LOG := valgrind.log

CC := gcc
CFLAGS := -O2 -g
ifdef SANITIZE
CFLAGS += -Og -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address
endif

WARNING := -Wall -Wextra
ifndef NOERROR
WARNING += -Werror
endif

NAME := ft_malcolm
ifdef SANITIZE
NAME := $(NAME)_asan
endif

OUT_DIR := out
ifdef SANITIZE
OUT_DIR := $(OUT_DIR)/sanitize
else
OUT_DIR := $(OUT_DIR)/release
endif

SRCS := src/malcolm.c src/arp.c

OBJS := $(SRCS:.c=.o)
OBJS := $(addprefix $(OUT_DIR)/, $(OBJS))

all: $(NAME)

-include $(OBJS:.o=.d)

$(OUT_DIR)/%.o: %.c Makefile
	$(MKDIR) $(@D)
	$(COMPILE.c) $< $(WARNING) -MMD -MP -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

asan-%: export SANITIZE = true
asan-%: export NOERROR = true
asan-%:
	$(MAKE) --no-print-directory $*

noerr-%: export NOERROR = true
noerr-%:
	$(MAKE) --no-print-directory $*

gdb:
	-@$(GDB) ./$(NAME)
valgrind:
	-@$(VALGRIND) --log-file=$(VALGRIND_LOG) ./$(NAME) $(ARGS)
	@$(LESS) $(VALGRIND_LOG)

clean: 
	$(RM) $(OUT_DIR)
fclean: clean
	$(RM) $(NAME)
re: fclean all

compile_commands:
	compiledb make re

pvs: compile_commands
	$(RM) pvs_report
	pvs-studio-analyzer analyze -f ./compile_commands.json
	plog-converter -t fullhtml PVS-Studio.log -o ./pvs_report

cc: compile_commands
	codechecker analyze --ctu ./compile_commands.json --output ./cc_report
	codechecker parse ./cc_report

.PHONY: all clean fclean re valgrind gdb pvs cc