# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS =

# Add pkg-config flags
CFLAGS += $(shell pkg-config --cflags sdl3 sdl3-image sdl3-ttf) -I/usr/include/SDL3_image
LDFLAGS += $(shell pkg-config --libs sdl3 sdl3-image sdl3-ttf)

# Project files
EXEC = sokoban
SRCS = main.c world.c world_parser.c world_view.c game.c
OBJS = main.o world.o world_parser.o world_view.o game.o

# Default target executed when you just run `make`
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Compile source files into object files
# This is a pattern rule that says how to build a .o file from a .c file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run the game
run: all
	./$(EXEC)

# Clean up build files
clean:
	rm -f $(EXEC) $(OBJS)

# Phony targets are rules that don't represent actual files
.PHONY: all clean run
