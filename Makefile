# Compiler and flags
CC      := gcc -ggdb
CFLAGS  := -Wall -Wextra -O2 -Iinclude
LDFLAGS := $(shell sdl2-config --libs)

# Project structure
SRC_DIR := src
OBJ_DIR := obj
TARGET  := game

# Collect all .c files in src/
SRCS := $(wildcard $(SRC_DIR)/*.c)
MAIN_SRCS := $(filter-out $(SRC_DIR)/test.c, $(SRCS))
# Replace src/file.c -> obj/file.o
OBJS := $(MAIN_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Compilation rule: obj/file.o from src/file.c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $@

# ----------------
# Test build
# ----------------
# All sources except main.c
TEST_SRCS := $(filter-out $(SRC_DIR)/main.c, $(SRCS))
# Convert to objects in obj/
TEST_OBJS := $(TEST_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

test: $(TEST_OBJS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $@ $(LDFLAGS)
	./$@

# ----------------
# Clean
# ----------------
clean:
	rm -rf $(OBJ_DIR) $(TARGET) test

.PHONY: all clean test

