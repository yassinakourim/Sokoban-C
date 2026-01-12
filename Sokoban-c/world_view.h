#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "world.h"

// Chemin vers l'image de la feuille de sprites.
static const char* SPRITESHEET_PATH = "assets/images/spritesheet/sprites.png";

// Tableaux de rectangles sources pour les différents sprites dans la feuille de sprites.
// Chaque SDL_FRect définit la position et la taille d'un sprite individuel.

// Sprites pour les murs.
static const SDL_FRect RECT_WALLS[] = {
    {0.f, 0.f, 64.f, 64.f}, {64.f, 0.f, 64.f, 64.f},
    {0.f, 64.f, 64.f, 64.f}, {64.f, 64.f, 64.f, 64.f},
    {0.f, 128.f, 64.f, 64.f}, 
    {0.f, 192.f, 64.f, 64.f}, 
    {0.f, 256.f, 64.f, 64.f}, 
    {0.f, 320.f, 64.f, 64.f}
};
static const int NUM_WALL_SPRITES = sizeof(RECT_WALLS) / sizeof(RECT_WALLS[0]);

// Sprites pour le sol.
static const SDL_FRect RECT_GROUNDS[] = {
    {128.f, 0.f, 64.f, 64.f}, {64.f, 128.f, 64.f, 64.f},
    {128.f, 64.f, 64.f, 64.f}, {64.f, 192.f, 64.f, 64.f},
    {128.f, 128.f, 64.f, 64.f}, {64.f, 256.f, 64.f, 64.f},
    {128.f, 192.f, 64.f, 64.f}, {64.f,320.f, 64.f, 64.f},
};
static const int NUM_GROUND_SPRITES = sizeof(RECT_GROUNDS) / sizeof(RECT_GROUNDS[0]);

// Sprites pour les caisses.
static const SDL_FRect RECT_CRATES[] = {
  {320.f, 64.f, 64.f, 64.f}, {320.f, 0.f, 64.f, 64.f},
  {256.f, 320.f, 64.f, 64.f}, {256.f, 256.f, 64.f, 64.f},
  {256.f, 192.f, 64.f, 64.f}, {256.f, 0.f, 64.f, 64.f},
  {256.f, 64.f, 64.f, 64.f}, {128.f, 256.f, 64.f, 64.f},
  {192.f, 64.f, 64.f, 64.f}, {192.f, 128.f, 64.f, 64.f},
  {192.f, 192.f, 64.f, 64.f}, {192.f, 256.f, 64.f, 64.f},
  {192.f, 320.f, 64.f, 64.f}, {256.f, 128.f, 64.f, 64.f},
  {128.f, 320.f, 64.f, 64.f}, {192.f, 0.f, 64.f, 64.f}
};
static const int NUM_CRATE_SPRITES = sizeof(RECT_CRATES) / sizeof(RECT_CRATES[0]);

// Sprites pour des objets spécifiques.
static const SDL_FRect RECT_CRATE_ON_GOAL = {128.f, 320.f, 64.f, 64.f};
static const SDL_FRect RECT_GOAL = {0.f, 384.f, 32.f, 32.f};

// Sprites pour les différentes animations du personnage.
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

// Structure pour la vue du monde, gérant l'affichage.
typedef struct {
  const World* world;           // Pointeur vers les données du monde à afficher.
  SDL_Texture* spritesheet;     // La texture de la feuille de sprites.
  uint8_t screen_tile_dim;      // Dimension d'une tuile à l'écran (en pixels).
  int wall_sprite_index;        // Index du sprite de mur actuel.
  int ground_sprite_index;      // Index du sprite de sol actuel.
  int crate_sprite_index;       // Index du sprite de caisse actuel.
} WorldView;

// Initialise la vue.
bool init_view(SDL_Renderer* renderer, WorldView* view, World* world, uint8_t screen_tile_dim);

// Affiche la vue à l'écran.
void render_view(SDL_Renderer* renderer, WorldView* view, Direction player_direction, bool player_is_moving);

// Nettoie les ressources de la vue.
void clean_view(WorldView* view);

// Sélectionne un thème de sprites aléatoire.
void randomize_theme(WorldView* view);