# Compiler and flags
CC       = gcc
CFLAGS   = -std=c11 -Wall -Iinclude -I$(TEST_DIR) -g
LDFLAGS  = -pthread
CHECK_LDFLAGS = -lcheck -lsubunit -lm

# Directories
SRC_DIR    = src
OBJ_DIR    = obj
BIN_DIR    = bin
TEST_DIR   = tests

# Application source and object files
SRCS = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/myshell

# Check test files
CHECK_SRCS = $(shell find $(TEST_DIR) -type f -name 'test_*.c')
CHECK_TARGET = $(BIN_DIR)/check_tests

# --------------------------------------------------------------------
# Default target
# --------------------------------------------------------------------
.PHONY: all
all: $(TARGET) $(CHECK_TARGET)

# Build main application
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------------------------
# Unit Tests using Check
# --------------------------------------------------------------------
.PHONY: tests check_tests runtests

tests: check_tests

check_tests: $(CHECK_TARGET)

$(CHECK_TARGET): $(CHECK_SRCS) $(filter-out $(SRC_DIR)/main.c, $(SRCS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(CHECK_LDFLAGS)

runtests: check_tests
	./$(CHECK_TARGET)

# --------------------------------------------------------------------
# Code formatting and static checks
# --------------------------------------------------------------------
.PHONY: format format-check cppcheck check

format:
	find $(SRC_DIR) $(TEST_DIR) include -type f \( -name "*.c" -o -name "*.h" \) \
		-exec clang-format -i {} +

format-check:
	find $(SRC_DIR) $(TEST_DIR) include -type f \( -name "*.c" -o -name "*.h" \) \
		-exec clang-format --dry-run --Werror {} +

cppcheck:
	cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem $(SRC_DIR) $(TEST_DIR)

check: format-check cppcheck

# --------------------------------------------------------------------
# Clean build artifacts
# --------------------------------------------------------------------
.PHONY: clean

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
