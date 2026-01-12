#include "world.h"
#include <stdio.h>

// Vecteurs cardinaux pour les directions (Nord, Est, Sud, Ouest).
// Utile pour traduire une énumération de direction en un changement de coordonnées (ligne, colonne).
static const int cardinal_vectors[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// Vérifie si le joueur a gagné.
// La condition de victoire est que chaque caisse se trouve sur une case objectif.
bool has_player_won(const World* world) {
  for (size_t line = 0; line < world->height; line += 1) {
    for (size_t col = 0; col < world->width; col += 1) {
      // Si une cellule contient une caisse...
      if (world->map[line][col] & FLAG_CRATE) {
        // ...mais ne contient pas d'objectif, alors le joueur n'a pas encore gagné.
        if (!(world->map[line][col] & FLAG_GOAL)) {
          return false;
        }
      }
    }
  }
  // Si toutes les caisses sont sur des objectifs, le joueur a gagné.
  return true;
}

// Vérifie si une position donnée est dans les limites du monde.
static bool is_position_valid(const World* world, int line, int col) {
  return line >= 0 && line < (int)world->height && col >= 0 &&
         col < (int)world->width;
}

// Vérifie si une position est valide et "vide" (ne contient ni mur, ni caisse, ni joueur).
// Une case objectif est considérée comme vide.
static bool is_position_valid_and_empty(const World* world, int line, int col) {
  return is_position_valid(world, line, col) &&
         (world->map[line][col] == EMPTY || world->map[line][col] == FLAG_GOAL);
}

// Déplace un objet (joueur ou caisse) d'une position à une autre.
void move_object(World* world, uint8_t flag_object, size_t old_line,
                 size_t old_col, size_t new_line, size_t new_col) {
  assert(is_position_valid(world, old_line, old_col));
  assert(is_position_valid(world, new_line, new_col));

  // Retire l'indicateur de l'objet de l'ancienne position.
  world->map[old_line][old_col] &= ~flag_object;
  // Ajoute l'indicateur de l'objet à la nouvelle position.
  world->map[new_line][new_col] |= flag_object;
}

// Gère la logique de déplacement du joueur.
bool move_player(World* world, Direction dir) {
  Position player_pos = find_player_position(world);
  if (player_pos.line == (size_t)-1) return false; // Joueur non trouvé

  int new_line = (int)player_pos.line + cardinal_vectors[dir][0];
  int new_col = (int)player_pos.col + cardinal_vectors[dir][1];

  if (!is_position_valid(world, new_line, new_col)) {
    return false; // Le joueur ne peut pas sortir du monde.
  }

  // Cas 1 : La case de destination est vide.
  if (is_position_valid_and_empty(world, new_line, new_col)) {
    move_object(world, FLAG_PLAYER, player_pos.line, player_pos.col, (size_t)new_line, (size_t)new_col);
    return true;
  }

  // Cas 2 : La case de destination contient une caisse.
  if (world->map[new_line][new_col] & FLAG_CRATE) {
    int behind_crate_line = new_line + cardinal_vectors[dir][0];
    int behind_crate_col = new_col + cardinal_vectors[dir][1];

    // Vérifie si la case derrière la caisse est vide.
    if (is_position_valid_and_empty(world, behind_crate_line, behind_crate_col)) {
      // Déplace la caisse, puis le joueur.
      move_object(world, FLAG_CRATE, (size_t)new_line, (size_t)new_col, (size_t)behind_crate_line, (size_t)behind_crate_col);
      move_object(world, FLAG_PLAYER, player_pos.line, player_pos.col, (size_t)new_line, (size_t)new_col);
      return true;
    }
  }

  return false; // Mouvement impossible (par exemple, dans un mur).
}


// Trouve la position du joueur dans le monde.
Position find_player_position(const World* world) {
  for (size_t line = 0; line < world->height; line += 1) {
    for (size_t col = 0; col < world->width; col += 1) {
      if (world->map[line][col] & FLAG_PLAYER) {
        return (Position){.line = line, .col = col};
      }
    }
  }

  // Retourne une position invalide si le joueur n'est pas trouvé.
  return (Position){.line = (size_t)-1, .col = (size_t)-1};
}
