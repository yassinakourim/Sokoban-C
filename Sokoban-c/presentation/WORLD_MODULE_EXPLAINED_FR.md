# Explication des Fichiers `world.h` et `world.c`

Ces deux fichiers sont responsables de la représentation et de la manipulation de l'état du monde du jeu. Ils gèrent la carte, la position des objets (murs, caisses, joueur) et la logique de mouvement fondamentale.

---

## `world.h` : Le Fichier d'En-tête

Ce fichier définit les "règles" et les structures de données pour le monde du jeu.

### Le Concept de Masque de Bits (Bitmask)

La partie la plus importante de ce fichier est la manière dont une cellule (case) de la carte est représentée. Au lieu d'avoir une simple valeur (par exemple, 0 pour vide, 1 pour mur), le projet utilise un **masque de bits**.

```c
static const uint8_t EMPTY = 0;
static const uint8_t FLAG_WALL = 1 << 0;   // Vaut 1 (binaire 0001)
static const uint8_t FLAG_GOAL = 1 << 1;   // Vaut 2 (binaire 0010)
static const uint8_t FLAG_CRATE = 1 << 2;  // Vaut 4 (binaire 0100)
static const uint8_t FLAG_PLAYER = 1 << 3; // Vaut 8 (binaire 1000)
```

Chaque "flag" (drapeau) est une puissance de 2, ce qui signifie qu'il occupe un bit unique dans un octet (`uint8_t`). Cela permet de combiner plusieurs états dans une seule cellule en utilisant l'opérateur `|` (OU bit à bit).

*   Une cellule vide vaut `0`.
*   Un mur vaut `1`.
*   Une caisse sur un objectif vaut `FLAG_CRATE | FLAG_GOAL` (c'est-à-dire `4 | 2`, ce qui donne `6` ou `0110` en binaire).

Cette méthode est très efficace en termes de mémoire et de performance.

### Structures de Données

*   **`World` (Structure)** : La structure principale qui contient l'état du monde.
    *   `map[100][100]` : Un tableau 2D d'octets (`uint8_t`) qui représente la carte du jeu. Chaque octet est un masque de bits comme décrit ci-dessus.
    *   `height`, `width` : Les dimensions réelles du niveau en cours, qui sont généralement plus petites que la taille maximale de 100x100.

*   **`Position` (Structure)** : Une structure simple pour stocker des coordonnées (`line` pour la ligne, `col` pour la colonne).

*   **`Direction` (Énumération)** : Définit les quatre directions cardinales.

### Déclarations de Fonctions

Le fichier déclare les trois fonctions publiques qui permettent aux autres modules (comme `game.c`) d'interagir avec le monde.

---

## `world.c` : L'Implémentation de la Logique du Monde

Ce fichier implémente la logique de jeu déclarée dans `world.h`.

### Fonctions d'Aide (Statiques)

Plusieurs fonctions `static` (internes au fichier) facilitent la logique :

*   **`cardinal_vectors`** : Un tableau qui associe chaque `Direction` à un changement de coordonnées (par ex. `NORTH` -> `{-1, 0}`).
*   **`is_position_valid(...)`** : Vérifie si une coordonnée est bien à l'intérieur des limites de la carte.
*   **`is_position_valid_and_empty(...)`** : Vérifie si une case est valide ET "vide", c'est-à-dire qu'elle ne contient ni mur ni caisse. Une case `FLAG_GOAL` est considérée comme vide.
*   **`move_object(...)`** : Une fonction très importante qui déplace un objet d'une case à une autre en manipulant les masques de bits :
    *   `world->map[ancienne_pos] &= ~flag_object;` : Utilise un ET (`&`) avec le complément (`~`) du drapeau pour **retirer** l'objet de l'ancienne case.
    *   `world->map[nouvelle_pos] |= flag_object;` : Utilise un OU (`|`) pour **ajouter** l'objet à la nouvelle case.

### Fonctions Publiques

*   **`find_player_position(const World* world)`** : Parcourt la carte (`world->map`) jusqu'à trouver la cellule qui contient le drapeau `FLAG_PLAYER` et retourne sa position.

*   **`has_player_won(const World* world)`** : Détermine si le niveau est terminé. Elle parcourt la carte et vérifie si chaque cellule contenant une caisse (`FLAG_CRATE`) contient **aussi** un objectif (`FLAG_GOAL`). Si elle trouve une seule caisse qui n'est pas sur un objectif, elle retourne `false`. Si toutes les caisses sont sur des objectifs, elle retourne `true`.

*   **`move_player(World* world, Direction dir)`** : C'est la fonction la plus complexe, qui contient la logique de mouvement principale.
    1.  Elle trouve d'abord la position actuelle du joueur.
    2.  Elle calcule la position de destination (`new_pos`) en fonction de la direction.
    3.  Elle analyse la case de destination :
        *   **Si la case est un mur** : Le mouvement est impossible. La fonction retourne `false`.
        *   **Si la case est vide (ou un objectif)** : Le mouvement est possible. La fonction appelle `move_object` pour déplacer le `FLAG_PLAYER` et retourne `true`.
        *   **Si la case contient une caisse (`FLAG_CRATE`)** : C'est une poussée.
            a. La fonction calcule la position de la case *derrière* la caisse.
            b. Si la case derrière la caisse est vide (ni un mur, ni une autre caisse), alors :
                i. Elle appelle `move_object` pour déplacer la caisse vers la case vide derrière elle.
                ii. Elle appelle `move_object` pour déplacer le joueur dans la case maintenant libérée par la caisse.
                iii. La fonction retourne `true`.
            c. Si la case derrière la caisse est occupée, la poussée est impossible. La fonction retourne `false`.
