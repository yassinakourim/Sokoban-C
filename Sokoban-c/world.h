#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define WORLD_MAX_DIM 100

static const uint8_t EMPTY = 0;
static const uint8_t FLAG_WALL = 1 << 0;
static const uint8_t FLAG_GOAL = 1 << 1;
static const uint8_t FLAG_CRATE = 1 << 2;
static const uint8_t FLAG_PLAYER = 1 << 3;

typedef enum {
  NORTH,
  EAST,
  SOUTH,
  WEST
} Direction;

typedef struct {
  uint8_t map[WORLD_MAX_DIM][WORLD_MAX_DIM];
  size_t height;
  size_t width;
} World;

typedef struct {
  size_t line;
  size_t col;
} Position;

bool has_player_won(const World* world);
bool move_player(World* world, Direction dir);
Position find_player_position(const World* world);