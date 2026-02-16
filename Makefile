# Compiler and flags
CC      := gcc
CFLAGS  := -ggdb -Wall -Wextra -O2 -Iinclude
LDFLAGS := $(shell sdl2-config --libs)

# Project structure
SRC_DIR := src
OBJ_DIR := obj
TARGET  := game

# Collect ALL .c files recursively
SRCS := $(shell find $(SRC_DIR) -name '*.c')

# Exclude test.c (anywhere)
MAIN_SRCS := $(filter-out $(SRC_DIR)/test.c,$(SRCS))

# Map src/path/file.c -> obj/path/file.o
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(MAIN_SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Compile rule with mirrored subdirs
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ----------------
# Test build
# ----------------
TEST_SRCS := $(filter-out $(SRC_DIR)/main.c,$(SRCS))
TEST_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))

test: $(TEST_OBJS)
	$(CC) $(TEST_OBJS) $(LDFLAGS) -o $@
	./$@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) test

.PHONY: all clean test

