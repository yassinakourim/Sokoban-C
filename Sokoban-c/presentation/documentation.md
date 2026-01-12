# Documentation du projet Sokoban

## Fichiers d'en-tête (.h)

### `best_score.h`

Ce fichier d'en-tête déclare les fonctions pour la gestion des meilleurs scores pour chaque niveau.

#### Fonctions

- `void update_best_score(size_t level_index, int score)`: Met à jour le meilleur score pour un niveau donné.
- `int get_best_score(size_t level_index)`: Récupère le meilleur score pour un niveau donné.

### `game.h`

Ce fichier d'en-tête définit les structures et les fonctions principales du jeu.

#### Structures

- `MainMenu`: Représente le menu principal du jeu, avec les textures pour les options "facile" et "difficile".
- `Game`: La structure principale du jeu, contenant l'état du jeu, le monde, la vue, le parseur de niveaux, la fenêtre, le rendu, etc.

#### Énumérations

- `Difficulty`: Représente la difficulté du jeu (facile ou difficile).
- `GameState`: Représente l'état actuel du jeu (menu principal, en jeu, niveau terminé, etc.).

#### Fonctions

- `int init_game(Game* game)`: Initialise le jeu.
- `void handle_game_events(Game* game)`: Gère les événements du jeu (entrées clavier, etc.).
- `void update_game(Game* game)`: Met à jour l'état du jeu.
- `void render_game(Game* game)`: Affiche le jeu à l'écran.
- `void clean_game(Game* game)`: Libère les ressources allouées pour le jeu.
- `int init_main_menu(Game* game, MainMenu* menu)`: Initialise le menu principal.
- `void handle_main_menu_events(Game* game, MainMenu* menu, SDL_Event* event)`: Gère les événements du menu principal.
- `void render_main_menu(Game* game, MainMenu* menu)`: Affiche le menu principal.
- `void clean_main_menu(MainMenu* menu)`: Libère les ressources allouées pour le menu principal.

### `world.h`

Ce fichier d'en-tête définit la structure du monde du jeu et les fonctions pour interagir avec lui.

#### Macros

- `WORLD_MAX_DIM`: La dimension maximale du monde du jeu (100).

#### Constantes

- `EMPTY`: Représente une case vide.
- `FLAG_WALL`: Représente un mur.
- `FLAG_GOAL`: Représente un objectif.
- `FLAG_CRATE`: Représente une caisse.
- `FLAG_PLAYER`: Représente le joueur.

#### Énumérations

- `Direction`: Représente les directions de déplacement (NORD, EST, SUD, OUEST).

#### Structures

- `World`: Représente le monde du jeu, avec une carte 2D, sa hauteur et sa largeur.
- `Position`: Représente les coordonnées d'une position (ligne, colonne).

#### Fonctions

- `bool has_player_won(const World* world)`: Vérifie si le joueur a gagné.
- `bool move_player(World* world, Direction dir)`: Déplace le joueur dans une direction donnée.
- `Position find_player_position(const World* world)`: Trouve la position du joueur dans le monde.

### `world_parser.h`

Ce fichier d'en-tête déclare les fonctions et les structures pour l'analyse des fichiers de niveaux.

#### Macros

- `MAX_LINE_SIZE`: La taille maximale d'une ligne dans un fichier de niveau.

#### Structures

- `WorldParser`: Représente l'analyseur de niveaux, avec le chemin du fichier de niveaux, le nombre de niveaux, le niveau actuel et le nom du niveau actuel.

#### Fonctions

- `bool init_parser(WorldParser* parser, const char* level_file_path)`: Initialise l'analyseur de niveaux.
- `bool load_level(WorldParser* parser, World* world)`: Charge un niveau à partir du fichier de niveaux.
- `void print_world(World* world)`: Affiche le monde dans la console.

### `world_view.h`

Ce fichier d'en-tête gère l'affichage du monde du jeu, y compris les sprites et les textures.

#### Constantes

- `SPRITESHEET_PATH`: Le chemin vers la feuille de sprites.
- `RECT_WALLS`, `RECT_GROUNDS`, `RECT_CRATES`, etc.: Des tableaux de rectangles définissant les positions des différents sprites dans la feuille de sprites.

#### Structures

- `WorldView`: Représente la vue du monde, avec le monde, la feuille de sprites, la dimension des tuiles, et les index des sprites pour le thème actuel.

#### Fonctions

- `bool init_view(SDL_Renderer* renderer, WorldView* view, World* world, uint8_t screen_tile_dim)`: Initialise la vue du monde.
- `void render_view(SDL_Renderer* renderer, WorldView* view, Direction player_direction, bool player_is_moving)`: Affiche la vue du monde.
- `void clean_view(WorldView* view)`: Libère les ressources allouées pour la vue du monde.
- `void randomize_theme(WorldView* view)`: Sélectionne un thème de sprites aléatoire.

## Fichiers sources (.c)

### `best_score.c`

Ce module gère la lecture et l'écriture des meilleurs scores dans un fichier.

#### Déroulement

- Le fichier `best_scores.txt` est utilisé pour stocker les meilleurs scores.
- `update_best_score`: Ouvre le fichier, recherche le niveau spécifié et met à jour le score s'il existe, sinon l'ajoute à la fin du fichier.
- `get_best_score`: Ouvre le fichier, recherche le niveau spécifié et retourne le score s'il est trouvé, sinon retourne -1.

### `game.c`

Ce module est le cœur du jeu. Il gère la boucle principale, les événements, la mise à jour de l'état du jeu et le rendu.

#### Déroulement

- **`init_game`**: Initialise la SDL, la SDL_ttf, la fenêtre, le rendu, et le menu principal.
- **`handle_game_events`**: Gère les événements globaux (comme la fermeture de la fenêtre) et les événements spécifiques à l'état du jeu (menu principal ou en jeu).
- **`update_game`**: Met à jour l'état du jeu en fonction de l'état actuel. Si le jeu est en cours, il vérifie si le joueur a gagné et charge le niveau suivant si nécessaire.
- **`render_game`**: Efface l'écran et affiche le contenu approprié en fonction de l'état du jeu (menu principal ou jeu).
- **`clean_game`**: Libère toutes les ressources allouées par le jeu.
- Le jeu est structuré comme une machine à états, avec les états `GAME_STATE_MAIN_MENU`, `GAME_STATE_PLAYING`, `GAME_STATE_LEVEL_COMPLETE`, et `GAME_STATE_GAME_OVER`.
- La boucle de jeu principale (dans `main.c`) appelle `handle_game_events`, `update_game`, et `render_game` à chaque image.

### `main.c`

C'est le point d'entrée du programme.

#### Déroulement

- **`main`**:
    - Initialise le générateur de nombres aléatoires.
    - Initialise le jeu en appelant `init_game`.
    - Exécute la boucle de jeu principale tant que `game.is_running` est vrai.
    - Appelle `clean_game` pour libérer les ressources avant de quitter.

### `world.c`

Ce module gère la logique du monde du jeu, y compris les déplacements et la détection de victoire.

#### Déroulement

- **`has_player_won`**: Parcourt la carte et vérifie si toutes les caisses sont sur des objectifs.
- **`move_player`**:
    - Trouve la position du joueur.
    - Calcule la nouvelle position en fonction de la direction.
    - Vérifie si la nouvelle position est valide.
    - Si la nouvelle position est vide, déplace le joueur.
    - Si la nouvelle position contient une caisse, vérifie si la position derrière la caisse est vide, et si c'est le cas, déplace la caisse et le joueur.
- **`find_player_position`**: Parcourt la carte et retourne la position du joueur.

### `world_parser.c`

Ce module est responsable de la lecture et de l'analyse des fichiers de niveaux.

#### Déroulement

- **`init_parser`**: Ouvre le fichier de niveaux, compte le nombre de niveaux (séparés par `~`) et initialise la structure `WorldParser`.
- **`load_level`**:
    - Ouvre le fichier de niveaux.
    - Se déplace jusqu'au début du niveau actuel.
    - Lit le nom du niveau.
    - Lit la carte du niveau ligne par ligne, convertissant les caractères de texte en "map bytes" (drapeaux binaires).
- **`map_byte_to_txt_char`** et **`txt_char_to_map_byte`**: Fonctions d'aide pour convertir entre les caractères de la carte et les drapeaux binaires.

### `world_view.c`

Ce module est responsable de l'affichage du monde du jeu.

#### Déroulement

- **`init_view`**: Charge la feuille de sprites et initialise la structure `WorldView`.
- **`render_view`**:
    - Parcourt chaque cellule de la carte du monde.
    - Affiche le sprite de sol approprié.
    - Affiche les murs, les objectifs, les caisses et le joueur en fonction des drapeaux de la cellule.
    - Sélectionne le sprite du joueur en fonction de sa direction et de son état de mouvement.
- **`clean_view`**: Libère la texture de la feuille de sprites.
- **`randomize_theme`**: Sélectionne des sprites aléatoires pour les murs, le sol et les caisses.
