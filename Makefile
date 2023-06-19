SHELL := /bin/bash

RM := rm -rf
MKDIR := mkdir -p
LESS := less
GDB := gdb
VALGRIND := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRIND_LOG := valgrind.log
COMPILEDB := compiledb
CODECHECKER := CodeChecker

CC := gcc

ifdef TEST
CPPFLAGS := -DTESTING
endif

CFLAGS := -g
ifdef DEBUG
CFLAGS += -Og
else
CFLAGS += -O2
endif
ifdef SANITIZE
CFLAGS += -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address,undefined
endif

ifdef TEST
LDFLAGS := -lcriterion
endif

WARNING := -Wall -Wextra
ifndef DEBUG
WARNING += -Werror
endif

NAME := malcolm
ifdef TEST
NAME := $(NAME)_test
else
ifdef SANITIZE
NAME := $(NAME)_asan
else
ifdef DEBUG
NAME := $(NAME)_debug
endif
endif
endif

OUT_DIR := out
ifdef SANITIZE
OUT_DIR := $(OUT_DIR)/sanitize
else
OUT_DIR := $(OUT_DIR)/release
endif

ifdef TEST
SRCS := src/args.c \
	tests/args.test.c \
	src/utils.c \
	tests/utils.test.c
else
SRCS := src/malcolm.c \
	src/arp.c \
	src/args.c \
	src/utils.c
endif

OBJS := $(SRCS:.c=.o)
OBJS := $(addprefix $(OUT_DIR)/, $(OBJS))

all: $(NAME)

-include $(OBJS:.o=.d)

$(OUT_DIR)/%.o: %.c Makefile
	$(MKDIR) $(@D)
	$(COMPILE.c) $< $(WARNING) -MMD -MP -o $@

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o $@

test: export DEBUG = true
test: export SANITIZE = true
test: export TEST = true
test:
	$(MAKE) --no-print-directory $*
	./$(NAME)_test

asan-%: export SANITIZE = true
asan-%: export DEBUG = true
asan-%:
	$(MAKE) --no-print-directory $*

debug-%: export DEBUG = true
debug-%:
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
	$(COMPILEDB) make re

cc: compile_commands
	$(CODECHECKER) analyze --ctu ./compile_commands.json --output ./cc_report
	$(CODECHECKER) parse ./cc_report

.PHONY: all clean fclean re valgrind gdb pvs cc