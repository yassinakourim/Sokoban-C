#include "world.h"
#include <stdio.h>

static const int cardinal_vectors[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

bool has_player_won(const World* world) {
  for (size_t line = 0; line < world->height; line += 1) {
    for (size_t col = 0; col < world->width; col += 1) {
      if (world->map[line][col] & FLAG_CRATE) {
        if (!(world->map[line][col] & FLAG_GOAL)) {
          return false;
        }
      }
    }
  }
  return true;
}

static bool is_position_valid(const World* world, int line, int col) {
  return line >= 0 && line < (int)world->height && col >= 0 &&
         col < (int)world->width;
}

static bool is_position_valid_and_empty(const World* world, int line, int col) {
  return is_position_valid(world, line, col) &&
         (world->map[line][col] == EMPTY || world->map[line][col] == FLAG_GOAL);
}

void move_object(World* world, uint8_t flag_object, size_t old_line,
                 size_t old_col, size_t new_line, size_t new_col) {
  assert(is_position_valid(world, old_line, old_col));
  assert(is_position_valid(world, new_line, new_col));

  world->map[old_line][old_col] &= ~flag_object;
  world->map[new_line][new_col] |= flag_object;
}

bool move_player(World* world, Direction dir) {
  for (size_t line = 0; line < world->height; line += 1) {
    for (size_t col = 0; col < world->width; col += 1) {
      if (world->map[line][col] & FLAG_PLAYER) {
        int new_line = (int)line + cardinal_vectors[dir][0];
        int new_col = (int)col + cardinal_vectors[dir][1];

        if (!is_position_valid(world, new_line, new_col)) {
          return false;
        }

        if (is_position_valid_and_empty(world, new_line, new_col)) {
          move_object(world, FLAG_PLAYER, line, col, (size_t)new_line,
                      (size_t)new_col);
          return true;
        }

        if (world->map[new_line][new_col] & FLAG_CRATE) {
          int behind_crate_line = new_line + cardinal_vectors[dir][0];
          int behind_crate_col = new_col + cardinal_vectors[dir][1];

          if (is_position_valid_and_empty(world, behind_crate_line,
                                          behind_crate_col)) {
            move_object(world, FLAG_CRATE, (size_t)new_line, (size_t)new_col,
                        (size_t)behind_crate_line, (size_t)behind_crate_col);
            move_object(world, FLAG_PLAYER, line, col, (size_t)new_line,
                        (size_t)new_col);
            return true;
          }
        }

        return false;
      }
    }
  }

  return false;
}

Position find_player_position(const World* world) {
  for (size_t line = 0; line < world->height; line += 1) {
    for (size_t col = 0; col < world->width; col += 1) {
      if (world->map[line][col] & FLAG_PLAYER) {
        return (Position){.line = line, .col = col};
      }
    }
  }

  return (Position){.line = -1, .col = -1};
}
