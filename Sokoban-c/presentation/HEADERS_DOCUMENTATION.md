# Documentation des Fichiers d'En-tête (.h)

## `best_score.h`
Ce fichier déclare les fonctions pour gérer les meilleurs scores par niveau.

### Fonctions
- `void update_best_score(size_t level_index, int score)`: Met à jour le meilleur score pour un niveau donné. Si le nouveau score est meilleur que l'ancien, il est enregistré.
- `int get_best_score(size_t level_index)`: Récupère le meilleur score enregistré pour un niveau spécifique. Retourne -1 si aucun score n'est trouvé.

## `game.h`
Ce fichier définit la structure principale du jeu, les états et les fonctions de gestion du jeu et du menu principal.

### Structures
- `MainMenu`: Contient les textures pour les options du menu principal et l'option sélectionnée.
- `Game`: La structure principale du jeu, encapsulant tous les composants (monde, vue, analyseur, fenêtre, rendu, état du jeu, etc.).

### Énumérations
- `Difficulty`: Définit les niveaux de difficulté (facile, difficile).
- `GameState`: Définit les différents états du jeu (menu principal, en jeu, niveau terminé, jeu terminé).

### Fonctions
- `int init_game(Game* game)`: Initialise tous les composants du jeu, y compris SDL, TTF et le menu principal.
- `void handle_game_events(Game* game)`: Traite tous les événements SDL (clavier, souris, quitter) en fonction de l'état actuel du jeu.
- `void update_game(Game* game)`: Met à jour la logique interne du jeu, gère les transitions d'état et vérifie les conditions de victoire.
- `void render_game(Game* game)`: Dessine l'état actuel du jeu à l'écran, y compris le monde et le HUD.
- `void clean_game(Game* game)`: Libère toutes les ressources allouées par le jeu avant de quitter.
- `int init_main_menu(Game* game, MainMenu* menu)`: Initialise les textures et l'état du menu principal.
- `void handle_main_menu_events(Game* game, MainMenu* menu, SDL_Event* event)`: Gère les interactions du joueur avec le menu principal.
- `void render_main_menu(Game* game, MainMenu* menu)`: Affiche le menu principal à l'écran.
- `void clean_main_menu(MainMenu* menu)`: Libère les ressources spécifiques au menu principal.

## `world.h`
Ce fichier définit la structure du monde du jeu et les fonctions de logique pour les interactions au sein de ce monde.

### Macros
- `WORLD_MAX_DIM`: Définit la dimension maximale (hauteur/largeur) du monde du jeu.

### Constantes
- `EMPTY`, `FLAG_WALL`, `FLAG_GOAL`, `FLAG_CRATE`, `FLAG_PLAYER`: Utilisées comme des drapeaux binaires pour représenter les différents éléments pouvant occuper une cellule de la carte.

### Énumérations
- `Direction`: Représente les quatre directions cardinales (Nord, Est, Sud, Ouest) pour les mouvements.

### Structures
- `World`: Représente la carte du jeu comme un tableau 2D de `uint8_t` (où chaque `uint8_t` est un bitmask), ainsi que sa hauteur et sa largeur.
- `Position`: Structure simple pour stocker les coordonnées (ligne, colonne) d'une position sur la carte.

### Fonctions
- `bool has_player_won(const World* world)`: Vérifie si toutes les caisses sont placées sur des objectifs, indiquant une victoire.
- `bool move_player(World* world, Direction dir)`: Tente de déplacer le joueur dans la direction spécifiée. Gère également le poussage des caisses.
- `Position find_player_position(const World* world)`: Localise et retourne la position actuelle du joueur sur la carte.

## `world_parser.h`
Ce fichier déclare la structure et les fonctions pour l'analyse des fichiers de niveaux.

### Macros
- `MAX_LINE_SIZE`: La taille maximale d'une ligne lue depuis le fichier de niveau.

### Structures
- `WorldParser`: Contient le chemin du fichier de niveaux, le nombre total de niveaux, le niveau actuel en cours et le nom du niveau actuel.

### Fonctions
- `bool init_parser(WorldParser* parser, const char* level_file_path)`: Initialise le parseur de niveaux en lisant le fichier pour déterminer le nombre total de niveaux.
- `bool load_level(WorldParser* parser, World* world)`: Charge le niveau spécifié par `current_level` dans la structure `World` fournie.
- `void print_world(World* world)`: Affiche une représentation textuelle du monde dans la console (principalement pour le débogage).

## `world_view.h`
Ce fichier définit les structures et les fonctions pour le rendu visuel du monde du jeu.

### Constantes
- `SPRITESHEET_PATH`: Chemin vers la feuille de sprites (`sprites.png`) utilisée pour le rendu.
- `RECT_WALLS`, `RECT_GROUNDS`, `RECT_CRATES`, `RECT_GOAL`, `RECT_CRATE_ON_GOAL`, `RECT_CHAR_*`: Tableaux de `SDL_FRect` définissant les zones des différents sprites dans la feuille de sprites.

### Macros
- `NUM_WALL_SPRITES`, `NUM_GROUND_SPRITES`, `NUM_CRATE_SPRITES`: Nombres de sprites disponibles pour les murs, le sol et les caisses, respectivement.
- `SYMB_EMPTY`, `SYMB_WALL`, etc.: Caractères symboliques utilisés dans les fichiers de niveaux pour représenter les éléments.

### Structures
- `WorldView`: Contient un pointeur vers le monde à afficher, la texture de la feuille de sprites, la dimension des tuiles à l'écran, et les index des sprites choisis pour le thème actuel.

### Fonctions
- `bool init_view(SDL_Renderer* renderer, WorldView* view, World* world, uint8_t screen_tile_dim)`: Initialise la vue, charge la feuille de sprites et associe le monde à visualiser.
- `void render_view(SDL_Renderer* renderer, WorldView* view, Direction player_direction, bool player_is_moving)`: Dessine tous les éléments du monde (sol, murs, objectifs, caisses, joueur) sur le renderer.
- `void clean_view(WorldView* view)`: Libère la texture de la feuille de sprites et les autres ressources de la vue.
- `void randomize_theme(WorldView* view)`: Sélectionne aléatoirement un jeu de sprites pour les murs, le sol et les caisses afin de varier l'apparence.
