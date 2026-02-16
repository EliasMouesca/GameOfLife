# FLAGS
CC      := gcc
CFLAGS  := -ggdb -Wall -Wextra -O2 -Iinclude
LDFLAGS := $(shell sdl2-config --libs)
VALGRINDFLAGS := --tool=memcheck --leak-check=yes --track-origins=yes --num-callers=12 -s --quiet

PWD := $(shell pwd)
SRC_DIR := src
OBJ_DIR := obj
TARGET  := game

SRCS := $(shell find $(SRC_DIR) -name '*.c')
MODULES_SRCS := $(filter-out %_test.c $(SRC_DIR)/main.c,$(SRCS))
MODULES := $(sort $(patsubst $(SRC_DIR)/%/,%,$(dir $(MODULES_SRCS))))
MAIN_SRC := $(SRC_DIR)/main.c

MODULES_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(MODULES_SRCS))
MAIN_OBJ := $(OBJ_DIR)/main.o

all: $(TARGET)

$(TARGET): $(MODULES_OBJS) $(MAIN_OBJ)
	$(CC) $(MODULES_OBJS) $(MAIN_OBJ) $(LDFLAGS) -o $@ && printf "\nCompilation successful!\n\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

TEST_SRCS := $(filter %_test.c,$(SRCS))
TEST_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))

test: $(TEST_OBJS) $(OBJS)
	$(CC) $(TEST_OBJS) $(OBJS) $(LDFLAGS) -o $@
	#./$@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) test test-*

.PHONY: all clean test test-*


# Magia rara porque make no deja usar dos veces la wildcard (%)
define build_test_rule
test-$1: $(MODULES_OBJS) $(OBJ_DIR)/$1/$1_test.o
	$(CC) $$^ $(LDFLAGS) -o $$@
	@printf "\n=== $1 test ===\n"
	valgrind $(VALGRINDFLAGS) $(PWD)/$$@ && printf "\nPassed!\n"
	@printf "===============\n\n"
	rm $$@
endef
$(foreach f,$(MODULES),$(eval $(call build_test_rule,$f)))
