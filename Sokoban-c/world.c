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

void move_player(World* world, Direction dir) {
  for (size_t line = 0; line < world->height; line += 1) {
    for (size_t col = 0; col < world->width; col += 1) {
      if (world->map[line][col] & FLAG_PLAYER) {
        int new_line = (int)line + cardinal_vectors[dir][0];
        int new_col = (int)col + cardinal_vectors[dir][1];

        // IMPORTANT: ne jamais indexer la map sans vérifier d'abord
        if (!is_position_valid(world, new_line, new_col)) return;

        // case vide ou goal
        if (is_position_valid_and_empty(world, new_line, new_col)) {
          move_object(world, FLAG_PLAYER, line, col, (size_t)new_line,
                      (size_t)new_col);
          return;
        }

        // crate devant
        if (world->map[new_line][new_col] & FLAG_CRATE) {
          int count = 0;
          int check_line = new_line;
          int check_col = new_col;

          // 1) compter les caisses
          while (is_position_valid(world, check_line, check_col) &&
                 (world->map[check_line][check_col] & FLAG_CRATE)) {
            check_line += cardinal_vectors[dir][0];
            check_col += cardinal_vectors[dir][1];
            count++;
          }

          // 2) la case après la dernière caisse doit être vide
          if (!is_position_valid_and_empty(world, check_line, check_col))
            return;

          // 3) déplacer les caisses (de la fin vers le joueur)
          for (int i = count - 1; i >= 0; i--) {
            int from_line = new_line + i * cardinal_vectors[dir][0];
            int from_col = new_col + i * cardinal_vectors[dir][1];
            int to_line = from_line + cardinal_vectors[dir][0];
            int to_col = from_col + cardinal_vectors[dir][1];

            move_object(world, FLAG_CRATE, (size_t)from_line, (size_t)from_col,
                        (size_t)to_line, (size_t)to_col);
          }

          // 4) déplacer le joueur
          move_object(world, FLAG_PLAYER, line, col, (size_t)new_line,
                      (size_t)new_col);
          return;
        }

        return;  // un seul joueur => dès qu’on l’a trouvé, on sort
      }
    }
  }
}
