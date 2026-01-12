#pragma once
#include "world.h"

// Définit la taille maximale d'une ligne lue dans le fichier de niveaux.
#define MAX_LINE_SIZE WORLD_MAX_DIM

// Structure pour gérer l'analyse des fichiers de niveaux.
typedef struct {
  const char* level_file_path;          // Chemin vers le fichier de niveaux.
  size_t number_of_levels;              // Nombre total de niveaux dans le fichier.
  size_t current_level;                 // Index du niveau actuel.
  char current_level_name[MAX_LINE_SIZE]; // Nom du niveau actuel.
} WorldParser;

// Initialise le parseur de monde.
bool init_parser(WorldParser* parser, const char* level_file_path);

// Charge un niveau spécifique depuis le fichier et le met dans la structure World.
bool load_level(WorldParser* parser, World* world);

// Affiche le monde dans la console (utile pour le débogage).
void print_world(World* world);