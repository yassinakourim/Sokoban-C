#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// Définit la dimension maximale du monde (largeur et hauteur).
#define WORLD_MAX_DIM 100

// Constantes de masque de bits pour représenter les objets sur la carte.
// L'utilisation de masques de bits permet à une seule cellule (un `uint8_t`) de contenir plusieurs types d'objets.
// Par exemple, une cellule peut être à la fois un objectif et contenir le joueur.
static const uint8_t EMPTY = 0;              // Cellule vide.
static const uint8_t FLAG_WALL = 1 << 0;     // Un mur.
static const uint8_t FLAG_GOAL = 1 << 1;     // Un emplacement objectif.
static const uint8_t FLAG_CRATE = 1 << 2;    // Une caisse.
static const uint8_t FLAG_PLAYER = 1 << 3;   // Le joueur.

// Énumération pour les directions de mouvement.
typedef enum {
  NORTH, // Nord
  EAST,  // Est
  SOUTH, // Sud
  WEST   // Ouest
} Direction;

// Structure représentant le monde du jeu.
typedef struct {
  // La carte est une grille 2D. Chaque cellule est un `uint8_t` contenant un masque de bits des objets.
  uint8_t map[WORLD_MAX_DIM][WORLD_MAX_DIM];
  size_t height; // Hauteur actuelle de la carte.
  size_t width;  // Largeur actuelle de la carte.
} World;

// Structure pour représenter une position (ligne, colonne) dans le monde.
typedef struct {
  size_t line;
  size_t col;
} Position;

// Vérifie si le joueur a gagné (toutes les caisses sont sur des objectifs).
bool has_player_won(const World* world);

// Déplace le joueur dans une direction donnée.
bool move_player(World* world, Direction dir);

// Trouve la position actuelle du joueur dans le monde.
Position find_player_position(const World* world);