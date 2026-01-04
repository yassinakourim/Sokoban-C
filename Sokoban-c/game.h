#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "world.h"
#include "world_parser.h"
#include "world_view.h"
#include "best_score.h"

typedef struct {
  SDL_Texture* easy_image;
  SDL_Texture* hard_image;
  int selected_option;
} MainMenu;

typedef enum {
  DIFFICULTY_EASY,
  DIFFICULTY_HARD,
} Difficulty;

typedef enum {
  GAME_STATE_MAIN_MENU,
  GAME_STATE_PLAYING,
  GAME_STATE_LEVEL_COMPLETE,
  GAME_STATE_GAME_OVER,
} GameState;

typedef struct {
  World world;
  WorldView view;
  WorldParser parser;
  MainMenu main_menu;

  SDL_Window* window;
  SDL_Renderer* renderer;

  GameState state;
  Difficulty difficulty;
  bool is_running;

  int steps;
  int best_score;
  TTF_Font* font;
  Direction player_direction;
  bool player_is_moving;
  Uint64 player_last_move_time;
} Game;

int init_game(Game* game);

void handle_game_events(Game* game);
void update_game(Game* game);
void render_game(Game* game);

void clean_game(Game* game);

// Main Menu functions (now declared in game.h)
int init_main_menu(Game* game, MainMenu* menu);
void handle_main_menu_events(Game* game, MainMenu* menu, SDL_Event* event);
void render_main_menu(Game* game, MainMenu* menu);
void clean_main_menu(MainMenu* menu);