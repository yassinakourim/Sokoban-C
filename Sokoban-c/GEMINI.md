# GEMINI.md

## Project Overview

This project is a classic Sokoban puzzle game implemented in C. It uses the SDL3 library for graphics and input handling, and the SDL3_image library for loading sprite textures.

The project is structured into several modules, each with a clear responsibility:

*   **`main.c`**: The main entry point of the application. It initializes the game world and starts the main loop.
*   **`game.c` / `game.h`**: This is the core of the game. It contains the main game loop, handles SDL events (like keyboard input), and orchestrates the overall game flow, including level progression.
*   **`world.c` / `world.h`**: Manages the game's state, including the map layout, player position, and crate locations. The game map is represented as a 2D array where each cell is a bitmask of flags (e.g., `FLAG_WALL`, `FLAG_PLAYER`), which is an efficient way to represent overlapping objects like a player on a goal.
*   **`world_parser.c` / `world_parser.h`**: Responsible for parsing level files from the `assets/levels` directory. It is designed to read multiple levels from a single text file, where levels are separated by a `~` character.
*   **`world_view.c` / `world_view.h`**: Handles all rendering. It iterates over the game world's map and draws the appropriate sprites to the screen using the loaded spritesheet (`sprites.png`).
*   **`assets/`**: Contains all game assets. Subdirectories hold level files (`assets/levels`) and the PNG spritesheet (`assets/images/spritesheet`).

## Building and Running

This project uses a `Makefile` to simplify and automate the build process. The following commands are the primary way to interact with the project:

*   **`make`** (or `make all`): Compiles all necessary source files and links them to create the `sokoban` executable. It intelligently recompiles only the files that have changed since the last build.
*   **`make run`**: Builds the project (if necessary) and then runs the game. This is the typical command to use for playing.
*   **`make clean`**: Deletes the `sokoban` executable and all temporary object files (`.o`), cleaning the directory of build artifacts.

## Development Conventions

*   **Language**: The project is written in C, adhering to the C11 standard.
*   **Build System**: A `Makefile` is used for building the project, with `gcc` as the compiler.
*   **Dependencies**: The project depends on the `SDL3` and `sdl3-image` development libraries. These are linked using `pkg-config` within the `Makefile` to automatically find the correct compiler and linker flags.
*   **Code Style**:
    *   Header files use `#pragma once` for include guards to prevent multiple inclusions.
    *   Internal functions (functions not exposed in header files) are declared as `static` to limit their scope to a single file.
    *   The code is organized into modules with clear separation of concerns (game logic, rendering, data parsing).
    *   The game world state is efficiently managed using bitmasks, allowing multiple object types to exist in the same map cell.
