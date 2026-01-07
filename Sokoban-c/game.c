#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

static bool load_current_level(Game* game) {
  game->steps = 0;
  game->best_score = get_best_score(game->parser.current_level);
  randomize_theme(&game->view);
  

  while (game->parser.current_level < game->parser.number_of_levels &&
         !load_level(&game->parser, &game->world)) {
    
    game->parser.current_level += 1;
    
  }

  return game->parser.current_level < game->parser.number_of_levels;
}

int init_game(Game* game) {
  memset(game, 0, sizeof(Game));
  
  game->view.spritesheet = NULL;
  game->state = GAME_STATE_MAIN_MENU;
  game->is_running = true;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    
    return 1;
  }

  if (!TTF_Init()) {
    
    return 1;
  }

  game->font = TTF_OpenFont("assets/fonts/font.ttf", 24);
  if (!game->font) {
    
    return 1;
  }

  game->steps = 0;

  game->window = SDL_CreateWindow("Sokoban", 960, 740, SDL_WINDOW_RESIZABLE);

  if (!game->window) {
    
    return 1;
  }

  game->renderer = SDL_CreateRenderer(game->window, NULL);

  if (!game->renderer) {
    
    return 1;
  }

  if (!init_main_menu(game, &game->main_menu)) {
    
    return 1;
  }

  return 0;
}

static void handle_playing_events(Game* game, SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.repeat == 0) {
      Position old_pos = find_player_position(&game->world);
      bool moved = false;

      switch (event->key.key) {
        case SDLK_ESCAPE:
          game->is_running = false;
          break;

        case SDLK_UP:
          moved = move_player(&game->world, NORTH);
          if (moved) game->player_direction = NORTH;
          break;

        case SDLK_DOWN:
          moved = move_player(&game->world, SOUTH);
          if (moved) game->player_direction = SOUTH;
          break;

        case SDLK_LEFT:
          moved = move_player(&game->world, WEST);
          if (moved) game->player_direction = WEST;
          break;

        case SDLK_RIGHT:
          moved = move_player(&game->world, EAST);
          if (moved) game->player_direction = EAST;
          break;

        case SDLK_R:
          load_current_level(game);
          break;

        case SDLK_T:
          randomize_theme(&game->view);
          break;

        case SDLK_M:
          game->state = GAME_STATE_MAIN_MENU;
          game->parser.number_of_levels = 0;
          break;
        case SDLK_B:
          game->state = GAME_STATE_MAIN_MENU;
          game->parser.number_of_levels = 0;
          break;

        case SDLK_N:
          if (game->parser.current_level < game->parser.number_of_levels - 1) {
            game->parser.current_level++;
            load_current_level(game);
          }
          break;
        case SDLK_P:
          if (game->parser.current_level > 0) {
            game->parser.current_level--;
            load_current_level(game);
          }
          break;
      }

      if (moved) {
        Position new_pos = find_player_position(&game->world);
        if (old_pos.line != new_pos.line || old_pos.col != new_pos.col) {
          game->steps++;
          game->player_is_moving = true;
          game->player_last_move_time = SDL_GetTicks();
        }
      }
    }
  }
}

void handle_game_events(Game* game) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      game->is_running = false;
    }

    switch (game->state) {
      case GAME_STATE_MAIN_MENU:
        handle_main_menu_events(game, &game->main_menu, &event);
        break;
      case GAME_STATE_PLAYING:
        handle_playing_events(game, &event);
        break;

      default:
        break;
    }
  }
}

static void update_playing(Game* game) {
  if (game->player_is_moving && SDL_GetTicks() - game->player_last_move_time > 200) {
    game->player_is_moving = false;
  }

  if (has_player_won(&game->world)) {
    if (game->best_score == -1 || game->steps < game->best_score) {
      update_best_score(game->parser.current_level, game->steps);
    }
    game->parser.current_level += 1;

    if (game->parser.current_level == game->parser.number_of_levels) {
      game->state = GAME_STATE_GAME_OVER;
    } else {
      load_current_level(game);
      print_world(&game->world);
    }
  }
}

static void start_playing(Game* game) {
  
  const char* level_file;
  if (game->difficulty == DIFFICULTY_EASY) {
    level_file = "assets/levels/easy_levels.txt";
  } else {
    level_file = "assets/levels/hard_levels.txt";
  }

  
  if (!init_parser(&game->parser, level_file)) {
    
    game->is_running = false;
    return;
  }

  

  
  if (!load_current_level(game)) {
    
    game->is_running = false;
    return;
  }

  
  

  print_world(&game->world);

  
  
  if (!init_view(game->renderer, &game->view, &game->world, 37)) {
    
    game->is_running = false;
    return;
  }
  game->player_direction = SOUTH;
  
}

void update_game(Game* game) {
  switch (game->state) {
    case GAME_STATE_PLAYING:
      if (game->parser.number_of_levels == 0) {
        start_playing(game);
      } else {
        update_playing(game);
      }
      break;

    default:
      break;
  }
}

static void render_hud(Game* game) {
  char text[128];
  if (game->best_score != -1) {
    sprintf(text, "Level: %zu/%zu | Steps: %d | Best: %d", game->parser.current_level + 1, game->parser.number_of_levels, game->steps, game->best_score);
  } else {
    sprintf(text, "Level: %zu/%zu | Steps: %d | Best: __", game->parser.current_level + 1, game->parser.number_of_levels, game->steps);
  }

  SDL_Color color = { 255, 255, 255, 255 };
  SDL_Surface* surface = TTF_RenderText_Solid(game->font, text, strlen(text), color);
  if (!surface) {
    
    return;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);
  if (!texture) {
    
    SDL_DestroySurface(surface);
    return;
  }

  SDL_FRect dest_rect = { 10, 10, (float)surface->w, (float)surface->h };
  SDL_RenderTexture(game->renderer, texture, NULL, &dest_rect);

  // Render controls text
  sprintf(text, "R: Restart | N: Skip | P: Previous | T: Theme | B: Menu");
  SDL_Surface* controls_surface = TTF_RenderText_Solid(game->font, text, strlen(text), color);
  if (!controls_surface) {
    
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    return;
  }

  SDL_Texture* controls_texture = SDL_CreateTextureFromSurface(game->renderer, controls_surface);
  if (!controls_texture) {
    
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(controls_surface);
    return;
  }
  
  SDL_FRect controls_dest_rect = { 10, 40, (float)controls_surface->w, (float)controls_surface->h };
  SDL_RenderTexture(game->renderer, controls_texture, NULL, &controls_dest_rect);


  SDL_DestroySurface(surface);
  SDL_DestroyTexture(texture);
  SDL_DestroySurface(controls_surface);
  SDL_DestroyTexture(controls_texture);
}

static void render_playing_state(Game* game) {
  
  render_view(game->renderer, &game->view, game->player_direction,
              game->player_is_moving);
  render_hud(game);
  
}

void render_game(Game* game) {
  
  SDL_SetRenderDrawColor(game->renderer, 100, 149, 237, 255);
  SDL_RenderClear(game->renderer);

  switch (game->state) {
    case GAME_STATE_MAIN_MENU:
      
      render_main_menu(game, &game->main_menu);
      break;
    case GAME_STATE_PLAYING:
      
      render_playing_state(game);
      break;

    default:
      break;
  }

  SDL_RenderPresent(game->renderer);
  
}

void clean_game(Game* game) {
  clean_main_menu(&game->main_menu);
  clean_view(&game->view);

  TTF_CloseFont(game->font);
  TTF_Quit();

  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
}

// Main Menu Implementations
int init_main_menu(Game* game, MainMenu* menu) {
  
  menu->easy_image = IMG_LoadTexture(game->renderer, "assets/images/menu/easy.png");
  if (!menu->easy_image) {
    
    return 0;
  }

  menu->hard_image = IMG_LoadTexture(game->renderer, "assets/images/menu/hard.png");
  if (!menu->hard_image) {
    
    return 0;
  }

  menu->selected_option = 0;

  
  return menu->easy_image && menu->hard_image;
}

void handle_main_menu_events(Game* game, MainMenu* menu, SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
      case SDLK_UP:
        menu->selected_option = (menu->selected_option + 1) % 2;
        break;
      case SDLK_DOWN:
        menu->selected_option = (menu->selected_option + 1) % 2;
        break;
      case SDLK_RETURN:
        if (menu->selected_option == 0) {
          game->difficulty = DIFFICULTY_EASY;
        } else {
          game->difficulty = DIFFICULTY_HARD;
        }
        game->state = GAME_STATE_PLAYING;
        break;
    }
  }
}

void render_main_menu(Game* game, MainMenu* menu) {
  
  int width, height;
  
  SDL_GetWindowSize(game->window, &width, &height);
  

  SDL_FRect easy_rect = {width / 2 - 64, height / 2 - 100, 128, 128}; // Assuming 128x128 images
  SDL_FRect hard_rect = {width / 2 - 64, height / 2 + 20, 128, 128}; // Assuming 128x128 images

  SDL_Texture* easy_render_texture = menu->easy_image;
  SDL_Texture* hard_render_texture = menu->hard_image;

  if (menu->selected_option == 0) {
    SDL_SetTextureColorMod(easy_render_texture, 255, 255, 0); // Highlight selected
    SDL_SetTextureColorMod(hard_render_texture, 255, 255, 255);
  } else {
    SDL_SetTextureColorMod(easy_render_texture, 255, 255, 255);
    SDL_SetTextureColorMod(hard_render_texture, 255, 255, 0); // Highlight selected
  }

  
  SDL_RenderTexture(game->renderer, easy_render_texture, NULL, &easy_rect);
  
  SDL_RenderTexture(game->renderer, hard_render_texture, NULL, &hard_rect);
  
}

void clean_main_menu(MainMenu* menu) {
  
  SDL_DestroyTexture(menu->easy_image);
  SDL_DestroyTexture(menu->hard_image);
  
}
