# Sokoban Project: A Deep Dive

## Introduction

Welcome to this detailed explanation of the Sokoban project. This document is designed to guide you through the entire codebase, file by file, and concept by concept. The goal is to give you a deep understanding of how this game works, from the C code that runs it to the SDL3 library that draws it on the screen.

We will assume a beginner's perspective, explaining fundamental concepts as we encounter them.

---

## Core Concepts for a C & SDL Game

Before we look at the individual files, let's understand three core concepts that are central to how this game is built.

### 1. The Game Loop

Nearly every video game is built around a concept called the **Game Loop**. It's a simple `while` loop that runs continuously as long as the game is active. In each "tick" or "frame" of the loop, the game does three things:

1.  **Handle Input (Events):** Check if the player has pressed any keys, clicked the mouse, or tried to close the window.
2.  **Update Game State:** Based on the input, change the state of the game. For example, if the player pressed the "up" arrow, update the player's position in the game world.
3.  **Render (Draw):** Clear the screen and redraw everything in its new position.

This loop runs so fast (usually 60 times per second) that it creates the illusion of smooth motion and interaction. You will see this exact pattern in `game.c`.

### 2. The `World` Data Structure

How do we store the game's state? In this project, we use a `struct` called `World`. A `struct` in C is a way to group related variables into a single, custom data type.

```c
// From world.h
typedef struct {
  uint8_t map[WORLD_MAX_DIM][WORLD_MAX_DIM];
  size_t height;
  size_t width;
} World;
```

*   `map`: This is the most important part. It's a 2D array that represents the game board. `uint8_t` is an unsigned integer that is exactly 8 bits long, perfect for what we're about to do.
*   `height` & `width`: These store the current dimensions of the level's map.

### 3. Bitmasking: The Magic of the `map`

You might expect the `map` array to store simple numbers, like `0` for empty, `1` for a wall, `2` for a player. This project uses a much more powerful and efficient technique called **bitmasking**.

An 8-bit integer (`uint8_t`) is made of 8 individual bits. We can treat each bit as a separate boolean "flag".

```c
// From world.h
static const uint8_t FLAG_WALL   = 1 << 0; // Binary: 00000001
static const uint8_t FLAG_GOAL   = 1 << 1; // Binary: 00000010
static const uint8_t FLAG_CRATE  = 1 << 2; // Binary: 00000100
static const uint8_t FLAG_PLAYER = 1 << 3; // Binary: 00001000
```

*   The `<<` operator is a "bitwise left shift". `1 << 3` means "take the number 1 and shift its bits 3 places to the left," which results in `8`. This is an easy way to create flags where only one bit is "on".

Because each object type is a single bit, a single map cell can represent multiple objects at once! For example, what if a player is standing on a goal square? We use the **bitwise OR** operator (`|`) to combine them.

`uint8_t cell = FLAG_PLAYER | FLAG_GOAL; // 00001000 | 00000010 = 00001010`

To check if a player is in that cell, we use the **bitwise AND** operator (`&`).

`if (cell & FLAG_PLAYER) { ... } // This will be true!`

This is a very common and efficient technique in game development and low-level programming.

---

## File-by-File Code Explanation

Now let's go through each file in the project.

### `Makefile`

This file automates compiling your code.

*   `CC = gcc`: Sets the compiler to `gcc`.
*   `CFLAGS = -Wall -Wextra -std=c11 -g`: These are compiler flags.
    *   `-Wall -Wextra`: Turns on all major warnings. This is good practice to catch potential bugs.
    *   `-std=c11`: Specifies that we are using the 2011 version of the C language standard.
    *   `-g`: Includes debugging information in the final executable, which is essential for debugging.
*   `CFLAGS += $(shell ...)` and `LDFLAGS += $(shell ...)`: These lines run the `pkg-config` command to automatically get the correct compiler and linker flags for the SDL3 libraries. This is why we don't have to manually specify where the SDL3 libraries are.
*   `all: $(EXEC)`: This is the default rule. When you type `make`, this is what runs. It says that its goal is to create the executable file (`$(EXEC)`).
*   `$(EXEC): $(OBJS)`: This rule says that to create the executable, you first need all the object files (`.o`). It then links them all together.
*   `%.o: %.c`: This is a pattern rule. It tells `make` how to create a `.o` (object) file from a `.c` (source) file. It compiles each source file individually.
*   `run: all`: A custom rule to first build the project, then run it.
*   `clean: ...`: A custom rule to delete all the files created during compilation.

### `main.c`

This is the simplest file, and the entry point for the whole program.

```c
#include "game.h"
#include "world.h"

int main() {
  World world;
  return run_game(&world);
}
```

*   `#include "game.h"` and `#include "world.h"`: Includes our own header files so we can use the `World` struct and the `run_game` function.
*   `int main()`: Every C program starts execution here.
*   `World world;`: This declares a variable named `world` of our custom `World` struct type. This allocates the memory for our entire game state.
*   `return run_game(&world);`: This is the most important line.
    *   It calls the `run_game` function, which is the heart of our application (defined in `game.c`).
    *   `&world`: The `&` operator means "address of". We are not passing a *copy* of the world; we are passing a **pointer** to our original `world` variable. This is crucial. It means `run_game` can directly modify the one and only game world.

### `game.h` & `game.c`

This is where the main application logic and the game loop reside.

**`game.h`**
```c
#pragma once
#include "world.h"

static const char* LEVEL_FILE = "assets/levels/level_set_1.txt";
int run_game(World* world);
```
*   `#pragma once`: A modern way to ensure this header file is only included once, preventing compilation errors.
*   `static const char* LEVEL_FILE = ...`: Defines a constant string for the path to our level file.
*   `int run_game(World* world);`: This is the **function prototype**. It tells the rest of the program that a function named `run_game` exists, that it returns an `int`, and that it takes one argument: a pointer to a `World` struct.

**`game.c`**

This file is long, so we'll break it down into its main sections.

**1. Initialization (`run_game` part 1)**
```c
// ... includes ...

int run_game(World* world) {
  WorldParser parser;
  if (!init_parser(&parser, LEVEL_FILE)) { /* error handling */ }
  if (!load_current_level(&parser, world)) { /* error handling */ }

  // ... SDL initialization ...
  if (SDL_Init(SDL_INIT_VIDEO) < 0) { /* error handling */ }

  SDL_Window* window = SDL_CreateWindow("Sokoban", 960, 740, SDL_WINDOW_RESIZABLE);
  if (!window) { /* error handling */ }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) { /* error handling */ }

  WorldView view;
  if (!init_view(renderer, &view, world, 37)) { /* error handling */ }

  // ... game loop starts here ...
```
*   First, it creates a `WorldParser` and uses it to load the level data into the `world` struct.
*   `SDL_Init(SDL_INIT_VIDEO)`: This initializes the SDL library, specifically telling it we need its video/graphics capabilities.
*   `SDL_CreateWindow(...)`: This creates the main window for our game with a title ("Sokoban"), width, height, and a flag to make it resizable. It returns a pointer to an `SDL_Window` object.
*   `SDL_CreateRenderer(...)`: This creates a "renderer". The renderer is what we use to draw things to the window. It's an abstraction that can handle hardware acceleration for drawing, making it very fast.
*   `init_view(...)`: This initializes our rendering module (`world_view.c`), passing it the renderer and a pointer to the world so it knows what to draw.

**2. The Game Loop (`run_game` part 2)**
```c
  bool is_game_running = true;
  while (is_game_running) {
    // Check for win condition
    if (has_player_won(world)) {
        // ... logic to load next level or quit ...
    }

    // Handle Input (Events)
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        is_game_running = false;
      }
      if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
          // ... cases for UP, DOWN, LEFT, RIGHT, R, ESCAPE ...
          // e.g., case SDLK_UP: move_player(world, NORTH); break;
        }
      }
    }

    // Update Game State (already done by move_player)

    // Render
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255); // Cornflower blue
    SDL_RenderClear(renderer);

    render_view(renderer, &view);

    SDL_RenderPresent(renderer);
  }
```
*   `while (is_game_running)`: Our main game loop.
*   `while (SDL_PollEvent(&event))`: This is the SDL event loop. `SDL_PollEvent` checks if any event (key press, mouse move, etc.) has happened since the last check. If so, it fills our `event` variable with the event's information and returns true. The `while` loop ensures we process *all* events that happened in a single frame.
*   `if (event.type == SDL_EVENT_QUIT)`: This checks if the user clicked the 'X' on the window.
*   `if (event.type == SDL_EVENT_KEY_DOWN)`: This checks if a key was pressed down. The `switch` statement then checks *which* key it was and calls the appropriate function, like `move_player`.
*   `SDL_SetRenderDrawColor(...)`: Sets the default drawing color.
*   `SDL_RenderClear(renderer)`: Clears the entire window to the color we just set.
*   `render_view(renderer, &view)`: This is our own function (from `world_view.c`) that does all the drawing of the level, player, and crates.
*   `SDL_RenderPresent(renderer)`: This is very important. SDL does all drawing on a "back buffer" (a hidden screen). This command swaps the back buffer with the front buffer (what the user sees), making all our changes from this frame visible at once.

**3. Cleanup (`run_game` part 3)**
```c
  // ... after the game loop ends ...
  clean_view(&view);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
```
*   It's crucial to clean up all the resources we created. We destroy the view, the renderer, and the window in the reverse order we created them.
*   `SDL_Quit()`: Shuts down all SDL subsystems.

### `world_parser.h` & `world_parser.c`

This module reads the level file from disk.

*   `init_parser()`: Opens the level file (`fopen`), reads through it once just to count the number of levels (by counting lines that start with `~`), and stores the count.
*   `load_level()`: This is the main parsing function.
    *   It seeks to the correct level in the file.
    *   It then reads the level line by line using `fgets`.
    *   For each character in a line, it calls `txt_char_to_map_byte` to convert text characters (`#`, `$`, `@`) into their corresponding bitmask flags.
    *   It stores these flags in the `world->map` 2D array.
*   `print_world()`: A useful debugging function that prints the world map to the console as text characters.

### `world.h` & `world.c`

This module contains the actual game rules and logic.

**`world.h`**
*   This file defines the `World` struct and all the bitmask `FLAG_` constants we discussed earlier. It also defines constants for the directions (`NORTH`, `EAST`, `SOUTH`, `WEST`).

**`world.c`**
*   `has_player_won()`: This function loops through every cell of the map. It checks if there is any `FLAG_CRATE` that is *not* also on a `FLAG_GOAL`. If it finds even one, the player has not won, and it returns `false`. If it finishes the loop without finding any, it returns `true`.
*   `move_player()`: This is the most complex logic in the game.
    1.  It loops through the map to find the player's current position (`line`, `col`).
    2.  It calculates the player's `new_line` and `new_col` based on the direction they want to move.
    3.  It checks if the new position is a valid spot on the map.
    4.  **Case 1: Empty Space.** If the new position is empty or a goal, it calls `move_object` to move the player. `move_object` simply turns "off" the `FLAG_PLAYER` bit at the old position and turns it "on" at the new position.
    5.  **Case 2: Crate.** If the new position contains a crate, it then looks *beyond* the crate in the same direction. If that space is empty, it first calls `move_object` to move the crate, and *then* calls `move_object` to move the player. It even handles pushing multiple crates in a row!

### `world_view.h` & `world_view.c`

This is the graphics module that puts everything on the screen.

**`world_view.h`**
*   `SPRITESHEET_PATH`: A constant for the path to the `sprites.png` file.
*   `SDL_FRect RECT_...`: These are very important. An `SDL_FRect` is a rectangle with floating-point coordinates (`x`, `y`, `width`, `height`). Each of these `RECT_` constants defines the exact rectangular area *on the spritesheet image* for a specific tile. For example, `RECT_WALL` tells SDL where to find the wall sprite within `sprites.png`.

**`world_view.c`**
*   `load_texture()`: This function uses a helper library, `SDL3_image`, to load our `sprites.png` file. `IMG_Load` can load many image formats (PNG, JPG, etc.). It then uses `SDL_CreateTextureFromSurface` to convert the loaded image into a hardware-accelerated `SDL_Texture`, which is much faster for drawing.
*   `init_view()`: Simply calls `load_texture` to get the spritesheet ready.
*   `render_view()`: This is the main drawing function.
    1.  It loops through every single cell of the `world->map`.
    2.  For each cell, it defines a `destination_rect`. This is the rectangle on the *screen* where we want to draw.
    3.  It then checks the flags for that cell (`if (world->map[line][col] & FLAG_WALL)`).
    4.  If it finds a flag, it calls `SDL_RenderTexture()`. This powerful function takes four arguments: the renderer, the texture to draw from (our spritesheet), the **source rectangle** (one of our `RECT_` constants from the header), and the **destination rectangle** (where on the screen to draw it).
    5.  This process repeats for every object in every cell, drawing the game world tile by tile.

---

This concludes our deep dive. I hope this detailed walkthrough helps you understand the project's structure and code. You now have a solid foundation to present this project and to start making improvements.