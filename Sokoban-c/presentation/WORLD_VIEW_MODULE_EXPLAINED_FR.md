# Explication des Fichiers `world_view.h` et `world_view.c`

Ces deux fichiers constituent le module de "vue" ou de rendu. Leur seule responsabilité est de dessiner l'état actuel du `World` à l'écran. Ils traduisent les données logiques du jeu (murs, caisses, joueur) en représentations graphiques visibles.

---

## `world_view.h` : Le Fichier d'En-tête

Ce fichier définit les structures de données et les constantes nécessaires pour le rendu.

### Le Principe de la Feuille de Sprites (Spritesheet)

L'élément central de ce module est le concept de "spritesheet". Au lieu de charger des dizaines d'images séparées, le jeu charge une seule image (`assets/images/spritesheet/sprites.png`) qui contient tous les éléments graphiques du jeu (tous les types de murs, de sols, les animations du personnage, etc.).

### Constantes `RECT_*`

Pour savoir quelle partie de la grande image (la spritesheet) correspond à quel objet, le code définit des tableaux de `SDL_FRect`. Un `SDL_FRect` est simplement un rectangle (x, y, largeur, hauteur) qui délimite une zone.

*   **`RECT_WALLS[]`**, **`RECT_GROUNDS[]`**, **`RECT_CRATES[]`** : Ce sont des tableaux de rectangles. Chacun de ces tableaux représente un "thème" visuel différent. Par exemple, `RECT_WALLS[0]` est le rectangle du premier style de mur sur la spritesheet, `RECT_WALLS[1]` est le deuxième, et ainsi de suite.
*   **`RECT_CHAR_*`** : Une série de rectangles qui pointent vers les différentes images (frames) d'animation du personnage : regardant vers le haut, vers le bas, en mouvement, à l'arrêt, etc.

### Structure `WorldView`

Cette structure contient toutes les informations nécessaires au module de rendu :

*   `const World* world` : Un pointeur vers les données du monde qu'il faut dessiner.
*   `SDL_Texture* spritesheet` : La feuille de sprites chargée en mémoire vidéo (GPU), prête à être utilisée pour le dessin. C'est un objet `SDL_Texture` très important.
*   `uint8_t screen_tile_dim` : La dimension en pixels d'une seule case du jeu une fois dessinée à l'écran.
*   `wall_sprite_index`, `ground_sprite_index`, `crate_sprite_index` : Ces trois indices déterminent quel thème est actuellement actif. Par exemple, si `wall_sprite_index` vaut `2`, alors le rendu utilisera le rectangle `RECT_WALLS[2]` pour dessiner tous les murs.

---

## `world_view.c` : L'Implémentation de la Logique de Rendu

Ce fichier implémente la logique de dessin.

### Analyse des Fonctions Clés

*   **`load_texture(...)`** : Une fonction d'aide qui charge une image depuis un fichier, la transforme en `SDL_Surface` (en mémoire vive/CPU), puis la convertit en `SDL_Texture` (en mémoire vidéo/GPU) pour un rendu rapide. C'est une pratique standard et efficace avec SDL.

*   **`init_view(...)`** : Initialise la vue. Elle charge la spritesheet en mémoire en appelant `load_texture` et configure les valeurs par défaut de la structure `WorldView`.

*   **`randomize_theme(...)`** : Change l'apparence du jeu. Elle choisit simplement de nouveaux indices aléatoires pour `wall_sprite_index`, `ground_sprite_index` et `crate_sprite_index`. Le changement est instantané au prochain rendu.

*   **`render_view(...)`** : C'est le moteur de rendu du jeu. Il est appelé à chaque image (frame) depuis la boucle de jeu principale.
    1.  Il effectue une double boucle `for` pour parcourir chaque cellule (`line`, `col`) de la carte (`view->world->map`).
    2.  **Calcul de la Destination** : Pour chaque cellule, il calcule d'abord le rectangle de destination à l'écran (`current_screen_pos`). C'est là que l'image sera dessinée.
    3.  **Dessin par Couches (Layering)** : Le rendu se fait par couches successives pour assurer que les objets apparaissent dans le bon ordre.
        a. **Couche 1 (Sol)** : Il dessine **toujours** un sprite de sol en premier. Le sprite de sol utilisé est déterminé par le `ground_sprite_index` actuel.
        b. **Couches d'Objets** : Ensuite, en utilisant les masques de bits, il vérifie ce que contient la cellule et dessine par-dessus le sol si nécessaire :
            *   Si la cellule contient `FLAG_WALL`, il dessine un mur.
            *   Si elle contient `FLAG_GOAL`, il dessine un objectif.
            *   Si elle contient `FLAG_CRATE`, il dessine une caisse (avec une texture spéciale si la caisse est sur un objectif).
            *   Si elle contient `FLAG_PLAYER`, il dessine le joueur. C'est la partie la plus complexe : elle utilise le `player_direction` et le `player_is_moving` pour choisir le bon rectangle (`RECT_CHAR_*`) sur la spritesheet, créant ainsi l'illusion d'animation et d'orientation.
    4.  Tout le dessin est réalisé avec la fonction `SDL_RenderTexture`, qui est extrêmement rapide car elle copie des textures déjà présentes en mémoire vidéo.
*   **`clean_view(...)`** : Une fonction de nettoyage simple qui libère la mémoire vidéo utilisée par la `spritesheet` en appelant `SDL_DestroyTexture`.
