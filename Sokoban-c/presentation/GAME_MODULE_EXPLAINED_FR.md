# Explication des Fichiers `game.h` et `game.c`

Ces deux fichiers constituent le cœur de la logique du jeu Sokoban. Ils gèrent l'état général du jeu, le cycle de vie de l'application (initialisation, boucle principale, nettoyage) et la coordination entre les différents modules (monde, rendu, analyseur de niveaux).

---

## `game.h` : Le Fichier d'En-tête

Le fichier `game.h` agit comme le plan directeur du module de jeu. Son rôle est de définir les **structures de données** essentielles et de déclarer les **fonctions publiques** qui peuvent être appelées par d'autres parties du programme, comme `main.c`.

### Structures de Données et Énumérations

`game.h` définit plusieurs types de données qui structurent l'état du jeu :

*   **`GameState` (Énumération)** : C'est une des parties les plus importantes. Elle définit les différents états dans lesquels le jeu peut se trouver, agissant comme une machine à états :
    *   `GAME_STATE_MAIN_MENU` : Le jeu est sur l'écran du menu principal.
    *   `GAME_STATE_PLAYING` : Le joueur est en train de jouer à un niveau.
    *   `GAME_STATE_LEVEL_COMPLETE` : Affiche un écran de transition (non utilisé dans ce code, mais prévu).
    *   `GAME_STATE_GAME_OVER` : Le joueur a terminé tous les niveaux.

*   **`Difficulty` (Énumération)** : Définit la difficulté choisie (`DIFFICULTY_EASY`, `DIFFICULTY_HARD`).

*   **`MainMenu` (Structure)** : Contient les données spécifiques au menu principal, comme les textures pour les images "Easy" et "Hard" et quelle option est actuellement sélectionnée.

*   **`Game` (Structure)** : C'est la structure de données principale et la plus centrale du projet. Elle rassemble **tout** l'état du jeu en un seul endroit :
    *   `world`, `view`, `parser`, `main_menu` : Des instances des autres modules principaux du jeu.
    *   `window`, `renderer` : Les pointeurs fondamentaux de la bibliothèque SDL pour la fenêtre et le rendu graphique.
    *   `state`, `difficulty` : L'état et la difficulté actuels du jeu, en utilisant les énumérations ci-dessus.
    *   `is_running` : Un booléen qui contrôle la boucle de jeu principale dans `main.c`.
    *   `steps`, `best_score` : Des entiers pour suivre les statistiques du niveau en cours, affichées dans le HUD (Heads-Up Display).
    *   `font` : La police de caractères chargée pour afficher du texte.
    *   `player_direction`, `player_is_moving`, `player_last_move_time` : Des variables pour gérer l'animation du personnage.

### Déclarations de Fonctions

Le fichier déclare les fonctions qui seront implémentées dans `game.c`. Cela inclut le cycle de vie principal et la gestion du menu.

---

## `game.c` : L'Implémentation de la Logique

Ce fichier contient le code qui donne vie aux déclarations de `game.h`. L'architecture principale est basée sur la machine à états définie par `GameState`.

### Le Principe de la Machine à États

La logique du jeu est séparée en fonction de l'état actuel (`game->state`). Les fonctions principales comme `handle_game_events`, `update_game`, et `render_game` utilisent de simples instructions `switch` pour appeler des sous-fonctions spécifiques à l'état courant. Par exemple, lorsque `game->state` est `GAME_STATE_PLAYING`, les événements, mises à jour et le rendu sont gérés différemment que lorsque l'état est `GAME_STATE_MAIN_MENU`.

### Analyse des Fonctions Clés

*   **`init_game(Game* game)`** : Initialise les bibliothèques SDL et SDL_ttf, crée la fenêtre et le moteur de rendu, charge la police, et initialise le menu principal. Elle définit l'état initial à `GAME_STATE_MAIN_MENU`.

*   **`handle_game_events(Game* game)`** : Contient la boucle `SDL_PollEvent` pour récupérer les entrées utilisateur. Elle vérifie d'abord si l'utilisateur veut fermer la fenêtre (`SDL_EVENT_QUIT`). Ensuite, elle délègue la gestion de l'événement à `handle_main_menu_events` ou `handle_playing_events` en fonction de l'état du jeu.

*   **`handle_playing_events(...)`** : Gère les pressions de touches lorsque le joueur est en jeu :
    *   **Flèches directionnelles** : Appellent `move_player` pour déplacer le joueur et les caisses.
    *   **'R'** : Recharge le niveau actuel.
    *   **'T'** : Change le thème visuel aléatoirement.
    *   **'M' / 'B'** : Retourne au menu principal.
    *   **'N' / 'P'** : Passe au niveau suivant ou précédent.

*   **`update_game(Game* game)`** : Met à jour la logique du jeu.
    *   Si le jeu vient de passer à l'état `GAME_STATE_PLAYING`, elle appelle `start_playing()` une seule fois pour charger le fichier de niveaux approprié et initialiser le monde du jeu.
    *   Sinon, elle appelle `update_playing()` qui vérifie si le joueur a gagné (`has_player_won`), met à jour le meilleur score, et charge le niveau suivant.

*   **`render_game(Game* game)`** : Responsable de tout le dessin.
    *   Elle efface l'écran avec une couleur de fond.
    *   Elle appelle la fonction de rendu appropriée (`render_main_menu` ou `render_playing_state`) en fonction de l'état.
    *   `render_playing_state` appelle à son tour `render_view` (pour dessiner le monde du jeu) et `render_hud` (pour afficher les informations textuelles comme le nombre de pas).
    *   Enfin, elle appelle `SDL_RenderPresent` pour afficher le résultat à l'écran.

*   **`clean_game(Game* game)`** : La fonction de nettoyage. Elle est appelée une seule fois à la fin du programme pour libérer proprement toutes les ressources : textures, police, moteur de rendu, fenêtre, et pour fermer les bibliothèques SDL.

*   **Fonctions du Menu Principal** : Les fonctions `init/handle/render/clean_main_menu` sont des implémentations spécifiques pour la logique du menu, comme charger les images, gérer la sélection avec les flèches, et surligner l'option choisie.
