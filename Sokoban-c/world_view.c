#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

#include "world_view.h"

// Fonction utilitaire pour charger une texture à partir d'un chemin de fichier.
SDL_Texture* load_texture(SDL_Renderer* renderer, const char* texture_path) {
  SDL_Texture* new_texture = NULL;
  SDL_Surface* image_surface = IMG_Load(texture_path);
  if (image_surface != NULL) {
    new_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_DestroySurface(image_surface);
  }
  return new_texture;
}

// Initialise la vue du monde.
bool init_view(SDL_Renderer* renderer, WorldView* view, World* world,
               uint8_t screen_tile_dim) {
  view->world = world;
  view->spritesheet = load_texture(renderer, SPRITESHEET_PATH);
  view->screen_tile_dim = screen_tile_dim;
  // Initialise les index de sprites à 0.
  view->wall_sprite_index = 0;
  view->ground_sprite_index = 0;
  view->crate_sprite_index = 0;

  return view->spritesheet != NULL;
}

// Affiche la vue du monde.
void render_view(SDL_Renderer* renderer, WorldView* view,
                 Direction player_direction, bool player_is_moving) {
  if (view == NULL || view->world == NULL) {
    return;
  }

  // Parcourt chaque cellule du monde.
  for (size_t line = 0; line < view->world->height; line += 1) {
    for (size_t col = 0; col < view->world->width; col += 1) {
      SDL_FRect current_screen_pos = {(float)(col * view->screen_tile_dim),
                                      (float)(line * view->screen_tile_dim),
                                      (float)view->screen_tile_dim,
                                      (float)view->screen_tile_dim};

      // Affiche toujours une tuile de sol en premier.
      SDL_RenderTexture(renderer, view->spritesheet, &RECT_GROUNDS[view->ground_sprite_index],
                     &current_screen_pos);

      uint8_t cell = view->world->map[line][col];

      // Affiche les objets en fonction des indicateurs de la cellule.
      if (cell & FLAG_WALL) {
        SDL_RenderTexture(renderer, view->spritesheet, &RECT_WALLS[view->wall_sprite_index],
                       &current_screen_pos);
      }

      if (cell & FLAG_GOAL) {
        SDL_RenderTexture(renderer, view->spritesheet, &RECT_GOAL,
                       &current_screen_pos);
      }

      if (cell & FLAG_CRATE) {
        if (cell & FLAG_GOAL) {
          SDL_RenderTexture(renderer, view->spritesheet, &RECT_CRATE_ON_GOAL,
                         &current_screen_pos);
        } else {
          SDL_RenderTexture(renderer, view->spritesheet, &RECT_CRATES[view->crate_sprite_index],
                         &current_screen_pos);
        }
      }

      // Gère l'affichage et l'animation du joueur.
      if (cell & FLAG_PLAYER) {
        const SDL_FRect* player_rect;
        // Sélectionne le sprite du joueur en fonction de sa direction et de son état de mouvement.
        switch (player_direction) {
          case NORTH:
            player_rect = player_is_moving ? &RECT_CHAR_9 : &RECT_CHAR_8;
            break;
          case SOUTH:
            player_rect = player_is_moving ? &RECT_CHAR_5 : &RECT_CHAR_6;
            break;
          case WEST:
            player_rect = player_is_moving ? &RECT_CHAR_1 : &RECT_CHAR_10;
            break;
          case EAST:
            player_rect = player_is_moving ? &RECT_CHAR_3 : &RECT_CHAR_2;
            break;
          default:
            player_rect = &RECT_CHAR_7; // Sprite par défaut
            break;
        }
        SDL_RenderTexture(renderer, view->spritesheet, player_rect,
                       &current_screen_pos);
      }
    }
  }
}

// Nettoie les ressources de la vue.
void clean_view(WorldView* view) {
  SDL_DestroyTexture(view->spritesheet);
  view->spritesheet = NULL;
}

// Sélectionne un thème (ensemble de sprites) aléatoire.
void randomize_theme(WorldView* view) {
  if (view == NULL) {
    return;
  }
  // Choisit des index de sprites aléatoires pour les murs, le sol et les caisses.
  view->wall_sprite_index = rand() % NUM_WALL_SPRITES;
  view->ground_sprite_index = rand() % NUM_GROUND_SPRITES;
  view->crate_sprite_index = rand() % NUM_CRATE_SPRITES;

  // Assure que les sprites ne sont pas les mêmes pour éviter les problèmes de visibilité.
  if (view->ground_sprite_index == view->wall_sprite_index) {
    view->ground_sprite_index = (view->ground_sprite_index + 1) % NUM_GROUND_SPRITES;
  }
  if (view->crate_sprite_index == view->wall_sprite_index) {
    view->crate_sprite_index = (view->crate_sprite_index + 1) % NUM_CRATE_SPRITES;
  }
}
