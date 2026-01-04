# Explication du Code Ligne par Ligne pour Présentation

## Partie 1: Introduction au Projet, Makefile et `main.c` (Personne 1)

### 1. Introduction au Projet

*   **Objectif :** Créer un jeu de Sokoban en C en utilisant la bibliothèque SDL3 pour les graphismes et les entrées.
*   **Structure :** Le projet est divisé en plusieurs modules, chacun ayant une responsabilité claire :
    *   `main.c` : Point d'entrée de l'application.
    *   `game.c` / `game.h` : Cœur du jeu, gère la boucle principale et les événements.
    *   `world.c` / `world.h` : Gère l'état du jeu (carte, joueur, caisses).
    *   `world_parser.c` / `world_parser.h` : Charge les niveaux à partir de fichiers.
    *   `world_view.c` / `world_view.h` : S'occupe de l'affichage graphique.
    *   `best_score.c` / `best_score.h` : Gère les meilleurs scores pour chaque niveau.
*   **Compilation :** Le projet est compilé à l'aide d'un `Makefile`.

### 2. Explication du `Makefile`

*   **`CC = gcc`** : Définit le compilateur à utiliser (GCC).
*   **`CFLAGS = -Wall -Wextra -std=c11 -g`** : Définit les options de compilation :
    *   `-Wall -Wextra` : Activent les avertissements pour détecter les erreurs potentielles.
    *   `-std=c11` : Spécifie la version du langage C à utiliser (C11).
    *   `-g` : Inclut les informations de débogage.
*   **`LDFLAGS =`** : Initialise les options de l'éditeur de liens.
*   **`CFLAGS += $(shell pkg-config --cflags sdl3 sdl3-image sdl3-ttf) -I/usr/include/SDL3_image`** : Ajoute les options de compilation pour les bibliothèques SDL3, SDL3_image et SDL3_ttf en utilisant `pkg-config`.
*   **`LDFLAGS += $(shell pkg-config --libs sdl3 sdl3-image sdl3-ttf)`** : Ajoute les options de l'éditeur de liens pour les bibliothèques SDL3, SDL3_image et SDL3_ttf.
*   **`EXEC = sokoban`** : Définit le nom de l'exécutable.
*   **`SRCS = main.c world.c world_parser.c world_view.c game.c best_score.c`** : Liste tous les fichiers source `.c`.
*   **`OBJS = main.o world.o world_parser.o world_view.o game.o best_score.o`** : Liste tous les fichiers objet `.o` correspondants.
*   **`all: $(EXEC)`** : La cible par défaut, qui dépend de l'exécutable.
*   **`$(EXEC): $(OBJS)`** : Règle pour créer l'exécutable à partir des fichiers objet.
*   **`%.o: %.c`** : Règle pour compiler un fichier `.c` en un fichier `.o`.
*   **`run: all`** : Règle pour compiler et exécuter le jeu.
*   **`clean: `** : Règle pour supprimer les fichiers générés par la compilation.
*   **`.PHONY: all clean run`** : Déclare les cibles qui ne sont pas des fichiers.

### 3. Explication du `main.c`

*   **`#include <stdlib.h>`** et **`#include <time.h>`** : Incluent les bibliothèques standard pour la génération de nombres aléatoires.
*   **`#include "game.h"`** : Inclut le fichier d'en-tête principal du jeu.
*   **`int main() { ... }`** : La fonction principale, point d'entrée du programme.
*   **`srand(time(NULL));`** : Initialise le générateur de nombres aléatoires avec le temps actuel pour garantir des séquences différentes à chaque exécution.
*   **`Game game;`** : Déclare une variable de type `Game` qui contiendra tout l'état du jeu.
*   **`if (init_game(&game) != 0) { ... }`** : Appelle la fonction d'initialisation du jeu. Si elle retourne une erreur (différent de 0), le programme se termine.
*   **`while (game.is_running) { ... }`** : La boucle de jeu principale, qui s'exécute tant que la variable `is_running` est vraie.
*   **`handle_game_events(&game);`** : Gère les entrées de l'utilisateur (clavier, etc.).
*   **`update_game(&game);`** : Met à jour l'état du jeu (position du joueur, etc.).
*   **`render_game(&game);`** : Affiche le jeu à l'écran.
*   **`clean_game(&game);`** : Libère toutes les ressources allouées avant de quitter.
*   **`return 0;`** : Indique que le programme s'est terminé avec succès.

## Partie 2: Logique du Jeu (`game.h`, `game.c`) (Personne 2)

### 1. Explication du `game.h`

*   **`#pragma once`** : Directive de préprocesseur pour éviter les inclusions multiples du même fichier d'en-tête.
*   **`#include <SDL3/SDL.h>`**, **`#include <SDL3_ttf/SDL_ttf.h>`** : Incluent les fichiers d'en-tête de SDL3 pour les fonctionnalités de base et la gestion des polices.
*   **`#include "world.h"`**, **`#include "world_parser.h"`**, **`#include "world_view.h"`**, **`#include "best_score.h"`** : Incluent les autres modules du projet.
*   **`typedef struct { ... } MainMenu;`** : Définit la structure du menu principal, contenant les textures pour les options "facile" et "difficile", et l'option sélectionnée.
*   **`typedef enum { ... } Difficulty;`** : Définit les niveaux de difficulté du jeu.
*   **`typedef enum { ... } GameState;`** : Définit les différents états du jeu (menu principal, en jeu, etc.).
*   **`typedef struct { ... } Game;`** : La structure principale du jeu, qui contient :
    *   `world`, `view`, `parser`, `main_menu` : Les structures des autres modules.
    *   `window`, `renderer` : La fenêtre et le moteur de rendu SDL.
    *   `state`, `difficulty` : L'état actuel et la difficulté du jeu.
    *   `is_running` : Un booléen qui contrôle la boucle principale du jeu.
    *   `steps`, `best_score` : Le nombre de pas et le meilleur score pour le niveau actuel.
    *   `font` : La police pour afficher le texte.
    *   `player_direction`, `player_is_moving`, `player_last_move_time` : Variables pour gérer l'animation du joueur.
*   **`int init_game(Game* game);`** et autres déclarations de fonctions : Prototypes des fonctions globales définies dans `game.c`.

### 2. Explication du `game.c`

*   **`static bool load_current_level(Game* game)`** :
    *   Réinitialise le nombre de pas (`game->steps = 0;`).
    *   Charge le meilleur score pour le niveau actuel (`game->best_score = get_best_score(...)`).
    *   Charge le niveau en utilisant `load_level` du module `world_parser`.
    *   Gère les niveaux invalides en passant au suivant.
*   **`int init_game(Game* game)`** :
    *   Initialise SDL, SDL_ttf, la fenêtre, le rendu et la police.
    *   Initialise l'état du jeu sur le menu principal.
*   **`static void handle_playing_events(Game* game, SDL_Event* event)`** :
    *   Gère les entrées du joueur pendant le jeu.
    *   Vérifie si la touche n'est pas maintenue enfoncée (`event->key.repeat == 0`).
    *   Appelle `move_player` en fonction de la touche de direction pressée.
    *   Incrémente le compteur de pas uniquement si le joueur a réellement bougé.
    *   Gère les autres touches (R, T, M, N, P).
*   **`void handle_game_events(Game* game)`** :
    *   La fonction principale de gestion des événements, qui appelle la fonction de gestion d'événements appropriée en fonction de l'état du jeu.
*   **`static void update_playing(Game* game)`** :
    *   Met à jour l'état du jeu lorsque le joueur est en train de jouer.
    *   Vérifie si le joueur a gagné (`has_player_won`).
    *   Si le joueur a gagné, met à jour le meilleur score si nécessaire et passe au niveau suivant.
*   **`static void start_playing(Game* game)`** :
    *   Initialise le `world_parser` avec le fichier de niveau approprié.
    *   Charge le premier niveau.
    *   Initialise la vue (`world_view`).
*   **`void update_game(Game* game)`** :
    *   La fonction principale de mise à jour, qui appelle la fonction de mise à jour appropriée en fonction de l'état du jeu.
*   **`static void render_hud(Game* game)`** :
    *   Affiche les informations du jeu (niveau, pas, meilleur score) et les contrôles.
    *   Affiche `__` si aucun meilleur score n'est enregistré.
*   **`static void render_playing_state(Game* game)`** :
    *   Appelle les fonctions de rendu pour le monde du jeu et le HUD.
*   **`void render_game(Game* game)`** :
    *   La fonction de rendu principale, qui efface l'écran et appelle la fonction de rendu appropriée en fonction de l'état du jeu.
*   **`void clean_game(Game* game)`** :
    *   Libère toutes les ressources allouées par SDL et le jeu.
*   **Fonctions du menu principal (`init_main_menu`, `handle_main_menu_events`, `render_main_menu`, `clean_main_menu`)** :
    *   Gèrent l'initialisation, les événements, le rendu et le nettoyage du menu principal.

## Partie 3: Gestion du Monde (`world.h`, `world.c`) (Personne 3)

### 1. Explication du `world.h`

*   **`#define WORLD_MAX_DIM 100`** : Définit la dimension maximale du monde du jeu.
*   **`static const uint8_t EMPTY = 0;`** et autres `FLAG_...` :
    *   Utilise un système de masques de bits (`bitwise flags`) pour représenter les objets sur la carte.
    *   Chaque drapeau (`flag`) est une puissance de 2, ce qui permet de combiner plusieurs objets dans une seule case (par exemple, un joueur sur une case de but).
    *   `uint8_t` est un entier non signé de 8 bits, ce qui est suffisant pour les 4 drapeaux.
*   **`typedef enum { NORTH, EAST, SOUTH, WEST } Direction;`** : Définit les quatre directions possibles pour le mouvement.
*   **`typedef struct { ... } World;`** :
    *   `map` : Une matrice 2D qui représente la carte du jeu. Chaque case contient un masque de bits des objets présents.
    *   `height`, `width` : Les dimensions de la carte.
*   **`typedef struct { ... } Position;`** : Une structure simple pour stocker les coordonnées d'une position sur la carte.
*   **`bool has_player_won(const World* world);`** et autres déclarations : Prototypes des fonctions définies dans `world.c`.

### 2. Explication du `world.c`

*   **`static const int cardinal_vectors[4][2] = { ... };`** : Un tableau de vecteurs pour faciliter le calcul des nouvelles positions en fonction d'une direction.
*   **`bool has_player_won(const World* world)`** :
    *   Parcourt toute la carte.
    *   Vérifie si toutes les caisses (`FLAG_CRATE`) sont sur des cases de but (`FLAG_GOAL`).
    *   Retourne `true` si c'est le cas, sinon `false`.
*   **`static bool is_position_valid(const World* world, int line, int col)`** :
    *   Vérifie si une position donnée est à l'intérieur des limites de la carte.
*   **`static bool is_position_valid_and_empty(const World* world, int line, int col)`** :
    *   Vérifie si une position est valide et si elle est vide ou une case de but (c'est-à-dire qu'un objet peut s'y déplacer).
*   **`void move_object(World* world, uint8_t flag_object, ...)`** :
    *   Déplace un objet (joueur ou caisse) d'une position à l'autre en manipulant les masques de bits.
    *   `&= ~flag_object` : Retire l'objet de l'ancienne position.
    *   `|= flag_object` : Ajoute l'objet à la nouvelle position.
*   **`bool move_player(World* world, Direction dir)`** :
    *   Trouve la position du joueur.
    *   Calcule la nouvelle position en fonction de la direction.
    *   **Logique de mouvement :**
        1.  Si la nouvelle case est vide, déplace le joueur et retourne `true`.
        2.  Si la nouvelle case contient une caisse :
            *   Calcule la position derrière la caisse.
            *   Si cette position est vide, déplace la caisse, puis le joueur, et retourne `true`.
        3.  Sinon (mur, ou caisse bloquée), ne fait rien et retourne `false`.
*   **`Position find_player_position(const World* world)`** :
    *   Parcourt la carte pour trouver la case qui contient le drapeau `FLAG_PLAYER`.
    *   Retourne la position du joueur.
    *   Retourne `(-1, -1)` si le joueur n'est pas trouvé (cas d'erreur).

## Partie 4: Analyse des Niveaux et Meilleurs Scores (`world_parser.h`, `world_parser.c`, `best_score.h`, `best_score.c`) (Personne 4)

### 1. Explication du `world_parser.h`

*   **`#define MAX_LINE_SIZE WORLD_MAX_DIM`** : Définit la taille maximale d'une ligne lue dans un fichier de niveau.
*   **`typedef struct { ... } WorldParser;`** :
    *   `level_file_path` : Chemin vers le fichier de niveaux.
    *   `number_of_levels` : Nombre total de niveaux dans le fichier.
    *   `current_level` : Index du niveau actuel.
    *   `current_level_name` : Nom du niveau actuel.
*   **`bool init_parser(WorldParser* parser, ...)`** et autres déclarations : Prototypes des fonctions définies dans `world_parser.c`.

### 2. Explication du `world_parser.c`

*   **`#define SYMB_EMPTY ' '`** et autres `SYMB_...` : Définit les caractères utilisés dans les fichiers de niveaux pour représenter les différents objets.
*   **`char map_byte_to_txt_char(uint8_t map_byte)`** : Convertit un `byte` de la carte en son caractère texte correspondant (utilisé pour `print_world`).
*   **`uint8_t txt_char_to_map_byte(char txt_char)`** : Convertit un caractère texte en son `byte` de carte correspondant (utilisé lors du chargement d'un niveau).
*   **`bool init_parser(WorldParser* parser, ...)`** :
    *   Ouvre le fichier de niveaux.
    *   Compte le nombre de niveaux en se basant sur le séparateur `~`.
    *   Stocke le nombre total de niveaux dans `parser->number_of_levels`.
*   **`bool load_level(WorldParser* parser, World* world)`** :
    *   Ouvre le fichier de niveaux.
    *   Se déplace jusqu'au début du niveau actuel en comptant les séparateurs `~`.
    *   Lit le nom du niveau.
    *   Lit la carte ligne par ligne, en convertissant les caractères en `bytes` de carte.
    *   Met à jour les dimensions `width` et `height` du monde.
*   **`void print_world(World* world)`** :
    *   Affiche la carte du monde dans la console (utile pour le débogage).

### 3. Explication du `best_score.h`

*   **`void update_best_score(size_t level_index, int score);`** : Prototype de la fonction pour mettre à jour le meilleur score.
*   **`int get_best_score(size_t level_index);`** : Prototype de la fonction pour récupérer le meilleur score.

### 4. Explication du `best_score.c`

*   **`#define BEST_SCORE_FILE "best_scores.txt"`** : Définit le nom du fichier où les meilleurs scores sont stockés.
*   **`void update_best_score(size_t level_index, int score)`** :
    *   Ouvre le fichier `best_scores.txt` en mode lecture/écriture (`r+`). S'il n'existe pas, il le crée (`w+`).
    *   Recherche si un score pour le niveau `level_index` existe déjà.
    *   Si un score existe, il se positionne à cet endroit et l'écrase avec le nouveau score.
    *   Sinon, il ajoute le nouveau score à la fin du fichier.
*   **`int get_best_score(size_t level_index)`** :
    *   Ouvre le fichier `best_scores.txt` en mode lecture.
    *   Parcourt le fichier pour trouver le score correspondant à `level_index`.
    *   Retourne le score s'il est trouvé, sinon retourne -1.

## Partie 5: Rendu Graphique (`world_view.h`, `world_view.c`) (Personne 5)

### 1. Explication du `world_view.h`

*   **`static const char* SPRITESHEET_PATH = ...`** : Définit le chemin vers l'image contenant tous les sprites du jeu (la "spritesheet").
*   **`static const SDL_FRect RECT_WALLS[] = { ... };`** et autres `RECT_...` :
    *   Définissent des rectangles (`SDL_FRect`) qui correspondent aux positions et dimensions des différents sprites dans la spritesheet.
    *   `RECT_WALLS`, `RECT_GROUNDS`, `RECT_CRATES` sont des tableaux pour permettre d'avoir plusieurs variations de sprites et de changer le thème du jeu.
    *   Les `RECT_CHAR_...` correspondent aux différents sprites du personnage pour l'animation de la marche dans les 4 directions.
*   **`static const int NUM_WALL_SPRITES = ...`** : Calcule le nombre de sprites de mur disponibles.
*   **`typedef struct { ... } WorldView;`** :
    *   `world` : Un pointeur vers le monde du jeu pour savoir quoi afficher.
    *   `spritesheet` : La texture SDL contenant tous les sprites.
    *   `screen_tile_dim` : La dimension en pixels d'une case du jeu à l'écran.
    *   `wall_sprite_index`, `ground_sprite_index`, `crate_sprite_index` : Les index des sprites à utiliser pour le thème actuel.
*   **`bool init_view(SDL_Renderer* renderer, ...)`** et autres déclarations : Prototypes des fonctions définies dans `world_view.c`.

### 2. Explication du `world_view.c`

*   **`SDL_Texture* load_texture(SDL_Renderer* renderer, ...)`** :
    *   Fonction utilitaire pour charger une image depuis un fichier et la convertir en une texture SDL, qui peut être affichée par le moteur de rendu.
*   **`bool init_view(SDL_Renderer* renderer, ...)`** :
    *   Initialise la structure `WorldView`.
    *   Charge la spritesheet en utilisant `load_texture`.
    *   Initialise les index de sprites pour le thème par défaut.
*   **`void render_view(SDL_Renderer* renderer, WorldView* view, ...)`** :
    *   La fonction de rendu principale.
    *   Parcourt chaque case de la carte du monde (`view->world->map`).
    *   **Pour chaque case :**
        1.  Affiche toujours un sprite de sol (`RECT_GROUNDS`).
        2.  Si la case contient `FLAG_WALL`, affiche un sprite de mur par-dessus.
        3.  Si la case contient `FLAG_GOAL`, affiche un sprite de but.
        4.  Si la case contient `FLAG_CRATE`, affiche un sprite de caisse (ou de caisse sur un but).
        5.  Si la case contient `FLAG_PLAYER`, affiche le sprite du joueur correspondant à sa direction et à son état de mouvement (`player_is_moving`).
*   **`void clean_view(WorldView* view)`** :
    *   Libère la mémoire utilisée par la texture de la spritesheet.
*   **`void randomize_theme(WorldView* view)`** :
    *   Choisit aléatoirement des index de sprites pour les murs, le sol et les caisses.
    *   Assure que les sprites de sol et de mur ne sont pas les mêmes pour une meilleure visibilité.
