# Explication Ligne par Ligne de `main.c`

Voici une explication détaillée ligne par ligne du fichier `main.c` du projet Sokoban.

```c
#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
  srand(time(NULL));
  Game game;

  if (init_game(&game) != 0) {
    return 1;
  }

  while (game.is_running) {
    handle_game_events(&game);
    update_game(&game);
    render_game(&game);
  }

  clean_game(&game);

  return 0;
}
```

### Explication Ligne par Ligne

*   **`#include <stdlib.h>`**: Inclut la bibliothèque standard du C (`stdlib.h`), qui fournit des fonctions utilitaires générales. Ici, elle est utilisée pour la fonction `srand`.
*   **`#include <time.h>`**: Inclut la bibliothèque `time.h`, utilisée ici pour obtenir l'heure actuelle via `time(NULL)`, ce qui sert de "germe" (seed) pour le générateur de nombres aléatoires.
*   **`#include "game.h"`**: Inclut le fichier d'en-tête personnalisé pour le module de jeu (`game.h`). Ce fichier déclare la structure `Game` et les fonctions (`init_game`, `handle_game_events`, `update_game`, `render_game`, `clean_game`) que `main.c` va utiliser.

*   **`int main() {`**: C'est le point d'entrée principal de tout programme C. L'exécution du programme commence toujours ici. Il est déclaré pour retourner un entier (`int`).

*   **`srand(time(NULL));`**: Cette ligne initialise le générateur de nombres aléatoires. En utilisant `time(NULL)` comme germe, les séquences de nombres aléatoires seront différentes à chaque exécution du programme.

*   **`Game game;`**: Déclare une variable nommée `game` du type `Game`. Cette structure `Game` est un conteneur central qui gère l'état complet du jeu (fenêtre SDL, moteur de rendu, position du joueur, données du niveau, etc.).

*   **`if (init_game(&game) != 0) {`**:
    *   Appelle la fonction `init_game`, en lui passant un *pointeur* vers la variable `game`.
    *   Le rôle de `init_game` est d'initialiser tout ce dont le jeu a besoin pour fonctionner : la bibliothèque SDL, la création de la fenêtre et du moteur de rendu, le chargement des ressources initiales, et la configuration de l'état initial du jeu.
    *   Selon une convention C courante, les fonctions d'initialisation retournent `0` en cas de succès et un nombre différent de zéro en cas d'échec. Cette instruction `if` vérifie si l'initialisation a échoué.

*   **`return 1;`**: Si `init_game` échoue, cette ligne est exécutée. Elle termine le programme et renvoie le code d'erreur `1` au système d'exploitation.

*   **`while (game.is_running) {`**:
    *   Ceci est la **boucle de jeu principale**. Le code à l'intérieur de cette boucle s'exécutera de manière répétée, image après image, tant que le drapeau `is_running` à l'intérieur de la structure `game` est vrai.
    *   Ce drapeau contrôle la durée de vie du jeu. Il est probablement initialisé à `true` dans `init_game` et sera mis à `false` lorsque l'utilisateur voudra quitter le jeu (par exemple, en fermant la fenêtre).

*   **`handle_game_events(&game);`**: À l'intérieur de la boucle, cette fonction est appelée en premier. Son rôle est de traiter les événements utilisateur, comme les pressions de touches du clavier, les clics de souris, ou la fermeture de la fenêtre du jeu.

*   **`update_game(&game);`**: Cette fonction met à jour la logique du jeu en fonction de l'état actuel. Par exemple, si le joueur a appuyé sur une touche de mouvement, c'est ici que sa position serait mise à jour dans les données du jeu.

*   **`render_game(&game);`**: Après avoir traité les entrées et mis à jour l'état du jeu, cette fonction est appelée pour dessiner tout à l'écran. Elle efface l'écran, dessine le niveau, le joueur, les caisses, les objectifs, puis affiche l'image finale à l'utilisateur.

*   **`}`**: Cette accolade ferme la boucle `while`. Si `game.is_running` est toujours vrai, l'exécution revient au début de la boucle. Si c'est faux, l'exécution continue après la boucle.

*   **`clean_game(&game);`**: Une fois que la boucle de jeu se termine (parce que `game.is_running` est devenu faux), cette fonction est appelée. Son but est de libérer toutes les ressources qui ont été allouées pendant le jeu (par exemple, détruire la fenêtre SDL, libérer les textures de la mémoire) afin d'éviter les fuites de mémoire.

*   **`return 0;`**: C'est la dernière ligne de la fonction `main`. Elle renvoie `0` au système d'exploitation, ce qui est le code standard pour une exécution réussie et sans erreur.

*   **`}`**: Cette accolade ferme la fonction `main`.