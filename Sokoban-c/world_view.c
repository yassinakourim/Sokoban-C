#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

#include "world_view.h"


SDL_Texture* load_texture(SDL_Renderer* renderer, const char* texture_path) {
  /* Inspired from
   * https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
   */
  SDL_Texture* new_texture = NULL;

  // Load image at specified path
  SDL_Surface* image_surface = IMG_Load(texture_path);
  if (image_surface == NULL) {
    
  } else {
    // Create texture from surface pixels
    new_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    if (new_texture == NULL) {
      
    }

    // Get rid of old loaded surface
    SDL_DestroySurface(image_surface);
  }

  return new_texture;
}

bool init_view(SDL_Renderer* renderer, WorldView* view, World* world,
               uint8_t screen_tile_dim) {
  view->world = world;
  view->spritesheet = load_texture(renderer, SPRITESHEET_PATH);
  view->screen_tile_dim = screen_tile_dim;
  view->wall_sprite_index = 0;
  view->ground_sprite_index = 0;
  view->crate_sprite_index = 0;

  if (view->spritesheet == NULL) return false;

  return true;
}

void render_view(SDL_Renderer* renderer, WorldView* view,
                 Direction player_direction, bool player_is_moving) {
  if (view == NULL || view->world == NULL) {
    
    return;
  }
  for (size_t line = 0; line < view->world->height; line += 1) {
    for (size_t col = 0; col < view->world->width; col += 1) {
      SDL_FRect current_screen_pos = {(float)(col * view->screen_tile_dim),
                                      (float)(line * view->screen_tile_dim),
                                      (float)view->screen_tile_dim,
                                      (float)view->screen_tile_dim};

      SDL_RenderTexture(renderer, view->spritesheet, &RECT_GROUNDS[view->ground_sprite_index],
                     &current_screen_pos);

      uint8_t cell = view->world->map[line][col];

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

      if (cell & FLAG_PLAYER) {
        const SDL_FRect* player_rect;
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
            player_rect = &RECT_CHAR_7;
            break;
        }
        SDL_RenderTexture(renderer, view->spritesheet, player_rect,
                       &current_screen_pos);
      }
    }
  }
}

void clean_view(WorldView* view) {
  SDL_DestroyTexture(view->spritesheet);
  view->spritesheet = NULL;
}

void randomize_theme(WorldView* view) {
  if (view == NULL) {
    return;
  }
  view->wall_sprite_index = rand() % NUM_WALL_SPRITES;
  view->ground_sprite_index = rand() % NUM_GROUND_SPRITES;
  view->crate_sprite_index = rand() % NUM_CRATE_SPRITES;

  if (view->ground_sprite_index == view->wall_sprite_index) {
    view->ground_sprite_index = (view->ground_sprite_index + 1) % NUM_GROUND_SPRITES;
  }
  if (view->crate_sprite_index == view->wall_sprite_index) {
    view->crate_sprite_index = (view->crate_sprite_index + 1) % NUM_CRATE_SPRITES;
  }
}
