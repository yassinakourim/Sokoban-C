#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "world.h"
#include "world_parser.h"
#include "world_view.h"
#include "best_score.h"

// Structure pour gérer le menu principal.
typedef struct {
  SDL_Texture* easy_image;   // Texture pour l'option "facile".
  SDL_Texture* hard_image;   // Texture pour l'option "difficile".
  int selected_option;     // Option actuellement sélectionnée (0 pour facile, 1 for difficile).
} MainMenu;

// Énumération pour les niveaux de difficulté du jeu.
typedef enum {
  DIFFICULTY_EASY, // Difficulté facile.
  DIFFICULTY_HARD, // Difficulté difficile.
} Difficulty;

// Énumération pour les différents états du jeu.
typedef enum {
  GAME_STATE_MAIN_MENU,       // Le jeu est dans le menu principal.
  GAME_STATE_PLAYING,         // Le joueur est en train de jouer.
  GAME_STATE_LEVEL_COMPLETE,  // Le joueur a terminé le niveau.
  GAME_STATE_GAME_OVER,       // Le jeu est terminé.
} GameState;

// La structure principale du jeu, contenant toutes les données relatives à l'état du jeu.
typedef struct {
  World world;         // Le monde du jeu (la carte, les caisses, le joueur).
  WorldView view;      // La vue du jeu (comment le monde est dessiné).
  WorldParser parser;  // Le parseur de niveau, pour charger les cartes.
  MainMenu main_menu;  // Le menu principal.

  SDL_Window* window;     // La fenêtre du jeu.
  SDL_Renderer* renderer; // Le moteur de rendu SDL.

  GameState state;         // L'état actuel du jeu (menu, en jeu, etc.).
  Difficulty difficulty; // La difficulté choisie par le joueur.
  bool is_running;         // Vrai tant que le jeu est en cours d'exécution.

  int steps;                       // Le nombre de pas du joueur dans le niveau actuel.
  int best_score;                  // Le meilleur score pour le niveau actuel.
  TTF_Font* font;                  // La police pour afficher du texte.
  Direction player_direction;      // La direction actuelle du joueur.
  bool player_is_moving;           // Vrai si le joueur est en train de bouger.
  Uint64 player_last_move_time;    // Le temps du dernier mouvement du joueur, pour l'animation.
} Game;

// Initialise l'état du jeu.
int init_game(Game* game);

// Gère les événements (entrées utilisateur).
void handle_game_events(Game* game);
// Met à jour l'état du jeu.
void update_game(Game* game);
// Affiche le jeu à l'écran.
void render_game(Game* game);

// Nettoie et libère les ressources du jeu.
void clean_game(Game* game);

// Fonctions du menu principal
int init_main_menu(Game* game, MainMenu* menu);
void handle_main_menu_events(Game* game, MainMenu* menu, SDL_Event* event);
void render_main_menu(Game* game, MainMenu* menu);
void clean_main_menu(MainMenu* menu);