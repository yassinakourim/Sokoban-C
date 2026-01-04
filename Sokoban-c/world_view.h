#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


#include "world.h"

static const char* SPRITESHEET_PATH = "assets/images/spritesheet/sprites.png";

static const SDL_FRect RECT_WALLS[] = {
    {0.f, 0.f, 64.f, 64.f}, {64.f, 0.f, 64.f, 64.f},
    {0.f, 64.f, 64.f, 64.f}, {64.f, 64.f, 64.f, 64.f},
    {0.f, 128.f, 64.f, 64.f}, 
    {0.f, 192.f, 64.f, 64.f}, 
    {0.f, 256.f, 64.f, 64.f}, 
    {0.f, 320.f, 64.f, 64.f}
};
static const int NUM_WALL_SPRITES = sizeof(RECT_WALLS) / sizeof(RECT_WALLS[0]);

static const SDL_FRect RECT_GROUNDS[] = {
    {128.f, 0.f, 64.f, 64.f}, {64.f, 128.f, 64.f, 64.f},
    {128.f, 64.f, 64.f, 64.f}, {64.f, 192.f, 64.f, 64.f},
    {128.f, 128.f, 64.f, 64.f}, {64.f, 256.f, 64.f, 64.f},
    {128.f, 192.f, 64.f, 64.f}, {64.f,320.f, 64.f, 64.f},
    
};
static const int NUM_GROUND_SPRITES = sizeof(RECT_GROUNDS) / sizeof(RECT_GROUNDS[0]);

static const SDL_FRect RECT_CRATES[] = {
  // CrateDark textures
  {320.f, 64.f, 64.f, 64.f},  // CrateDark_Beige
  {320.f, 0.f, 64.f, 64.f},   // CrateDark_Black
  {256.f, 320.f, 64.f, 64.f}, // CrateDark_Blue
  {256.f, 256.f, 64.f, 64.f}, // CrateDark_Brown
  {256.f, 192.f, 64.f, 64.f}, // CrateDark_Gray
  {256.f, 0.f, 64.f, 64.f},   // CrateDark_Purple
  {256.f, 64.f, 64.f, 64.f},  // CrateDark_Red
  {128.f, 256.f, 64.f, 64.f}, // CrateDark_Yellow

  // Standard Crate textures
  {192.f, 64.f, 64.f, 64.f},  // Crate_Beige
  {192.f, 128.f, 64.f, 64.f}, // Crate_Black
  {192.f, 192.f, 64.f, 64.f}, // Crate_Blue
  {192.f, 256.f, 64.f, 64.f}, // Crate_Brown
  {192.f, 320.f, 64.f, 64.f}, // Crate_Gray
  {256.f, 128.f, 64.f, 64.f}, // Crate_Purple
  {128.f, 320.f, 64.f, 64.f}, // Crate_Red
  {192.f, 0.f, 64.f, 64.f}    // Crate_Yellow
};
static const int NUM_CRATE_SPRITES = sizeof(RECT_CRATES) / sizeof(RECT_CRATES[0]);

static const SDL_FRect RECT_CRATE_ON_GOAL = {128.f, 320.f, 64.f, 64.f};
static const SDL_FRect RECT_GOAL = {0.f, 384.f, 32.f, 32.f};
static const SDL_FRect RECT_CHAR_1 = {320.f, 186.f, 42.f, 59.f};
static const SDL_FRect RECT_CHAR_2 = {320.f, 245.f, 42.f, 59.f};
static const SDL_FRect RECT_CHAR_3 = {320.f, 128.f, 42.f, 58.f};
static const SDL_FRect RECT_CHAR_4 = {362.f, 248.f, 37.f, 59.f};
static const SDL_FRect RECT_CHAR_5 = {320.f, 362.f, 37.f, 59.f};
static const SDL_FRect RECT_CHAR_6 = {357.f, 362.f, 37.f, 59.f};
static const SDL_FRect RECT_CHAR_7 = {384.f, 0.f, 37.f, 60.f};
static const SDL_FRect RECT_CHAR_8 = {362.f, 188.f, 37.f, 60.f};
static const SDL_FRect RECT_CHAR_9 = {362.f, 128.f, 37.f, 60.f};
static const SDL_FRect RECT_CHAR_10 = {320.f, 304.f, 42.f, 58.f};

#define SYMB_EMPTY ' '
#define SYMB_WALL '#'
#define SYMB_CRATE '$'
#define SYMB_GOAL '.'
#define SYMB_CRATE_ON_GOAL '*'
#define SYMB_PLAYER '@'
#define SYMB_PLAYER_ON_GOAL '+'

typedef struct {
  const World* world;
  SDL_Texture* spritesheet;
  uint8_t screen_tile_dim;
  int wall_sprite_index;
  int ground_sprite_index;
  int crate_sprite_index;
} WorldView;

bool init_view(SDL_Renderer* renderer, WorldView* view, World* world,
               uint8_t screen_tile_dim);
void render_view(SDL_Renderer* renderer, WorldView* view,
                 Direction player_direction, bool player_is_moving);
void clean_view(WorldView* view);
void randomize_theme(WorldView* view);