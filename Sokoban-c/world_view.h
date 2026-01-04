#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


#include "world.h"

static const char* SPRITESHEET_PATH = "assets/images/spritesheet/sprites.png";

static const SDL_FRect RECT_GROUND = {128.f, 64.f, 64.f, 64.f};
static const SDL_FRect RECT_WALL = {0.f, 256.f, 64.f, 64.f};
static const SDL_FRect RECT_CRATE = {192.f, 256.f, 64.f, 64.f};
static const SDL_FRect RECT_CRATE_ON_GOAL = {128.f, 320.f, 64.f, 64.f};
static const SDL_FRect RECT_GOAL = {0.f, 384.f, 32.f, 32.f};
static const SDL_FRect RECT_PLAYER_DOWN = {320.f, 186.f, 42.f, 59.f};
static const SDL_FRect RECT_PLAYER_UP = {320.f, 245.f, 42.f, 59.f};
static const SDL_FRect RECT_PLAYER_LEFT = {362.f, 128.f, 37.f, 60.f};
static const SDL_FRect RECT_PLAYER_RIGHT = {320.f, 186.f, 42.f, 59.f};

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
} WorldView;

bool init_view(SDL_Renderer* renderer, WorldView* view, World* world,
               uint8_t screen_tile_dim);
void render_view(SDL_Renderer* renderer, WorldView* view,
                 Direction player_direction);
void clean_view(WorldView* view);