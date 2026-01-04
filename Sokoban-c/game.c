#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

static bool load_current_level(Game* game) {
  printf("Loading level %zu...\n", game->parser.current_level);

  while (game->parser.current_level < game->parser.number_of_levels &&
         !load_level(&game->parser, &game->world)) {
    printf("Level %zu is not valid, skipping.\n", game->parser.current_level);
    game->parser.current_level += 1;
    printf("Loading level %zu...\n", game->parser.current_level);
  }

  return game->parser.current_level < game->parser.number_of_levels;
}

int init_game(Game* game) {
  memset(game, 0, sizeof(Game));
  printf("init_game: start\n");
  game->view.spritesheet = NULL;
  game->state = GAME_STATE_MAIN_MENU;
  game->is_running = true;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    printf("SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  if (!TTF_Init()) {
    printf("TTF_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  game->font = TTF_OpenFont("assets/fonts/font.ttf", 24);
  if (!game->font) {
    printf("Failed to load font: %s\n", SDL_GetError());
    return 1;
  }

  game->score = 0;

  game->window = SDL_CreateWindow("Sokoban", 960, 740, SDL_WINDOW_RESIZABLE);

  if (!game->window) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  game->renderer = SDL_CreateRenderer(game->window, NULL);

  if (!game->renderer) {
    printf("Could not create renderer: %s\n", SDL_GetError());
    return 1;
  }

  if (!init_main_menu(game, &game->main_menu)) {
    printf("Could not create main menu. Abort.\n");
    return 1;
  }

  return 0;
}

static void handle_playing_events(Game* game, SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
      case SDLK_ESCAPE:
        game->is_running = false;
        break;

      case SDLK_UP:
        move_player(&game->world, NORTH);
        game->player_direction = NORTH;
        break;

      case SDLK_DOWN:
        move_player(&game->world, SOUTH);
        game->player_direction = SOUTH;
        break;

      case SDLK_LEFT:
        move_player(&game->world, WEST);
        game->player_direction = WEST;
        break;

      case SDLK_RIGHT:
        move_player(&game->world, EAST);
        game->player_direction = EAST;
        break;

      case SDLK_R:
        load_current_level(game);
        break;
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

static void update_playing_state(Game* game) {
  if (has_player_won(&game->world)) {
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
  printf("start_playing: start\n");
  const char* level_file;
  if (game->difficulty == DIFFICULTY_EASY) {
    level_file = "assets/levels/easy_levels.txt";
  } else {
    level_file = "assets/levels/hard_levels.txt";
  }

  printf("start_playing: before init_parser\n");
  if (!init_parser(&game->parser, level_file)) {
    printf("Invalid level file %s. Abort.\n", level_file);
    game->is_running = false;
    return;
  }

  printf("Level file '%s' contains %zu level(s).\n", level_file,
         game->parser.number_of_levels);

  printf("start_playing: before load_current_level\n");
  if (!load_current_level(game)) {
    printf("No level could be loaded. Abort.\n");
    game->is_running = false;
    return;
  }

  printf("Level %zu loaded! Level name: '%s'\n", game->parser.current_level,
         game->parser.current_level_name);
  printf("start_playing: World dimensions: width=%zu, height=%zu, map=%p\n",
         game->world.width, game->world.height, (void*)game->world.map);

  print_world(&game->world);

  printf("start_playing: Address of game->world: %p\n", (void*)&game->world);
  printf("start_playing: before init_view\n");
  if (!init_view(game->renderer, &game->view, &game->world, 37)) {
    printf("Could not create view. Abort.\n");
    game->is_running = false;
    return;
  }
  game->player_direction = SOUTH;
  printf("start_playing: end\n");
}

void update_game(Game* game) {
  switch (game->state) {
    case GAME_STATE_PLAYING:
      if (game->parser.number_of_levels == 0) {
        start_playing(game);
      } else {
        update_playing_state(game);
      }
      break;

    default:
      break;
  }
}

static void render_hud(Game* game) {
  char text[128];
  sprintf(text, "Level: %zu/%zu | Score: %d", game->parser.current_level + 1, game->parser.number_of_levels, game->score);

  SDL_Color color = { 255, 255, 255, 255 };
  SDL_Surface* surface = TTF_RenderText_Solid(game->font, text, strlen(text), color);
  if (!surface) {
    printf("Failed to create text surface: %s\n", SDL_GetError());
    return;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);
  if (!texture) {
    printf("Failed to create text texture: %s\n", SDL_GetError());
    SDL_DestroySurface(surface);
    return;
  }

  SDL_FRect dest_rect = { 10, 10, (float)surface->w, (float)surface->h };
  SDL_RenderTexture(game->renderer, texture, NULL, &dest_rect);

  SDL_DestroySurface(surface);
  SDL_DestroyTexture(texture);
}

static void render_playing_state(Game* game) {
  printf("render_playing_state: start\n");
  render_view(game->renderer, &game->view, game->player_direction);
  render_hud(game);
  printf("render_playing_state: end\n");
}

void render_game(Game* game) {
  printf("render_game: start\n");
  SDL_SetRenderDrawColor(game->renderer, 100, 149, 237, 255);
  SDL_RenderClear(game->renderer);

  switch (game->state) {
    case GAME_STATE_MAIN_MENU:
      printf("render_game: GAME_STATE_MAIN_MENU\n");
      render_main_menu(game, &game->main_menu);
      break;
    case GAME_STATE_PLAYING:
      printf("render_game: GAME_STATE_PLAYING\n");
      render_playing_state(game);
      break;

    default:
      break;
  }

  SDL_RenderPresent(game->renderer);
  printf("render_game: end\n");
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
  printf("init_main_menu: start\n");
  menu->easy_image = IMG_LoadTexture(game->renderer, "assets/images/menu/easy.png");
  if (!menu->easy_image) {
    printf("Failed to load easy image: %s\n", SDL_GetError());
    return 0;
  }

  menu->hard_image = IMG_LoadTexture(game->renderer, "assets/images/menu/hard.png");
  if (!menu->hard_image) {
    printf("Failed to load hard image: %s\n", SDL_GetError());
    return 0;
  }

  menu->selected_option = 0;

  printf("init_main_menu: end\n");
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
  printf("render_main_menu: start\n");
  int width, height;
  printf("render_main_menu: before SDL_GetWindowSize\n");
  SDL_GetWindowSize(game->window, &width, &height);
  printf("render_main_menu: after SDL_GetWindowSize, width=%d, height=%d\n", width, height);

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

  printf("render_main_menu: before rendering easy_image\n");
  SDL_RenderTexture(game->renderer, easy_render_texture, NULL, &easy_rect);
  printf("render_main_menu: before rendering hard_image\n");
  SDL_RenderTexture(game->renderer, hard_render_texture, NULL, &hard_rect);
  printf("render_main_menu: end\n");
}

void clean_main_menu(MainMenu* menu) {
  printf("clean_main_menu: start\n");
  SDL_DestroyTexture(menu->easy_image);
  SDL_DestroyTexture(menu->hard_image);
  printf("clean_main_menu: end\n");
}