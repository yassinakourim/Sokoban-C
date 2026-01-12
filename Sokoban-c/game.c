#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

// Charge le niveau actuel.
static bool load_current_level(Game* game) {
  game->steps = 0;
  game->best_score = get_best_score(game->parser.current_level);
  randomize_theme(&game->view);
  
  // Tente de charger le niveau actuel. S'il échoue, passe au suivant.
  while (game->parser.current_level < game->parser.number_of_levels &&
         !load_level(&game->parser, &game->world)) {
    game->parser.current_level += 1;
  }

  // Renvoie vrai si un niveau a été chargé avec succès.
  return game->parser.current_level < game->parser.number_of_levels;
}

// Initialise le jeu.
int init_game(Game* game) {
  memset(game, 0, sizeof(Game));
  
  game->view.spritesheet = NULL;
  game->state = GAME_STATE_MAIN_MENU;
  game->is_running = true;

  // Initialise SDL.
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return 1;
  }

  // Initialise SDL_ttf pour le texte.
  if (!TTF_Init()) {
    return 1;
  }

  // Charge la police.
  game->font = TTF_OpenFont("assets/fonts/font.ttf", 24);
  if (!game->font) {
    return 1;
  }

  game->steps = 0;

  // Crée la fenêtre du jeu.
  game->window = SDL_CreateWindow("Sokoban", 960, 740, SDL_WINDOW_RESIZABLE);
  if (!game->window) {
    return 1;
  }

  // Crée le moteur de rendu.
  game->renderer = SDL_CreateRenderer(game->window, NULL);
  if (!game->renderer) {
    return 1;
  }

  // Initialise le menu principal.
  if (!init_main_menu(game, &game->main_menu)) {
    return 1;
  }

  return 0;
}

// Gère les événements lorsque le joueur est en train de jouer.
static void handle_playing_events(Game* game, SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.repeat == 0) {
      Position old_pos = find_player_position(&game->world);
      bool moved = false;

      // Gestion des touches du clavier.
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

        case SDLK_R: // Recommence le niveau.
          load_current_level(game);
          break;

        case SDLK_T: // Change le thème.
          randomize_theme(&game->view);
          break;

        case SDLK_M: // Retourne au menu.
        case SDLK_B:
          game->state = GAME_STATE_MAIN_MENU;
          game->parser.number_of_levels = 0;
          break;

        case SDLK_N: // Niveau suivant.
          if (game->parser.current_level < game->parser.number_of_levels - 1) {
            game->parser.current_level++;
            load_current_level(game);
          }
          break;
        case SDLK_P: // Niveau précédent.
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

// Gère les événements du jeu.
void handle_game_events(Game* game) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      game->is_running = false;
    }

    // Aiguille les événements en fonction de l'état du jeu.
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

// Met à jour la logique du jeu lorsque le joueur joue.
static void update_playing(Game* game) {
  if (game->player_is_moving && SDL_GetTicks() - game->player_last_move_time > 200) {
    game->player_is_moving = false;
  }

  // Vérifie si le joueur a gagné.
  if (has_player_won(&game->world)) {
    if (game->best_score == -1 || game->steps < game->best_score) {
      update_best_score(game->parser.current_level, game->steps);
    }
    game->parser.current_level += 1;

    // Passe au niveau suivant ou termine le jeu.
    if (game->parser.current_level == game->parser.number_of_levels) {
      game->state = GAME_STATE_GAME_OVER;
    } else {
      load_current_level(game);
      print_world(&game->world);
    }
  }
}

// Démarre une partie.
static void start_playing(Game* game) {
  const char* level_file;
  if (game->difficulty == DIFFICULTY_EASY) {
    level_file = "assets/levels/easy_levels.txt";
  } else {
    level_file = "assets/levels/hard_levels.txt";
  }

  // Initialise le parseur de niveaux.
  if (!init_parser(&game->parser, level_file)) {
    game->is_running = false;
    return;
  }

  // Charge le premier niveau.
  if (!load_current_level(game)) {
    game->is_running = false;
    return;
  }

  print_world(&game->world);

  // Initialise la vue du jeu.
  if (!init_view(game->renderer, &game->view, &game->world, 37)) {
    game->is_running = false;
    return;
  }
  game->player_direction = SOUTH;
}

// Met à jour l'état du jeu.
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

// Affiche l'ATH (Heads-Up Display).
static void render_hud(Game* game) {
  char text[128];
  // Affiche le niveau, les pas et le meilleur score.
  if (game->best_score != -1) {
    sprintf(text, "Level: %zu/%zu | Steps: %d | Best: %d", game->parser.current_level + 1, game->parser.number_of_levels, game->steps, game->best_score);
  } else {
    sprintf(text, "Level: %zu/%zu | Steps: %d | Best: __", game->parser.current_level + 1, game->parser.number_of_levels, game->steps);
  }

  SDL_Color color = { 255, 255, 255, 255 };
  SDL_Surface* surface = TTF_RenderText_Solid(game->font, text, strlen(text), color);
  if (!surface) return;

  SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);
  if (!texture) {
    SDL_DestroySurface(surface);
    return;
  }

  SDL_FRect dest_rect = { 10, 10, (float)surface->w, (float)surface->h };
  SDL_RenderTexture(game->renderer, texture, NULL, &dest_rect);

  // Affiche les contrôles.
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

// Affiche l'état de jeu "en jeu".
static void render_playing_state(Game* game) {
  render_view(game->renderer, &game->view, game->player_direction, game->player_is_moving);
  render_hud(game);
}

// Affiche le jeu.
void render_game(Game* game) {
  SDL_SetRenderDrawColor(game->renderer, 100, 149, 237, 255);
  SDL_RenderClear(game->renderer);

  // Affiche l'écran approprié en fonction de l'état du jeu.
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

// Nettoie les ressources du jeu.
void clean_game(Game* game) {
  clean_main_menu(&game->main_menu);
  clean_view(&game->view);

  TTF_CloseFont(game->font);
  TTF_Quit();

  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
}

// Implémentations du menu principal
int init_main_menu(Game* game, MainMenu* menu) {
  // Charge les images du menu.
  menu->easy_image = IMG_LoadTexture(game->renderer, "assets/images/menu/easy.png");
  if (!menu->easy_image) return 0;

  menu->hard_image = IMG_LoadTexture(game->renderer, "assets/images/menu/hard.png");
  if (!menu->hard_image) return 0;

  menu->selected_option = 0;
  return 1;
}

void handle_main_menu_events(Game* game, MainMenu* menu, SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
      case SDLK_UP:
      case SDLK_DOWN:
        menu->selected_option = (menu->selected_option + 1) % 2;
        break;
      case SDLK_RETURN: // Confirme la sélection.
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

  // Positionne les images du menu.
  SDL_FRect easy_rect = {width / 2 - 64, height / 2 - 100, 128, 128};
  SDL_FRect hard_rect = {width / 2 - 64, height / 2 + 20, 128, 128};

  // Met en surbrillance l'option sélectionnée.
  if (menu->selected_option == 0) {
    SDL_SetTextureColorMod(menu->easy_image, 255, 255, 0);
    SDL_SetTextureColorMod(menu->hard_image, 255, 255, 255);
  } else {
    SDL_SetTextureColorMod(menu->easy_image, 255, 255, 255);
    SDL_SetTextureColorMod(menu->hard_image, 255, 255, 0);
  }

  SDL_RenderTexture(game->renderer, menu->easy_image, NULL, &easy_rect);
  SDL_RenderTexture(game->renderer, menu->hard_image, NULL, &hard_rect);
}

void clean_main_menu(MainMenu* menu) {
  SDL_DestroyTexture(menu->easy_image);
  SDL_DestroyTexture(menu->hard_image);
}
