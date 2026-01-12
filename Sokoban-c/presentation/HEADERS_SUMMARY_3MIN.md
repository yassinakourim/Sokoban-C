# Résumé des Fichiers d'En-tête (.h) pour une Présentation de 3 minutes

Ce document explique brièvement le rôle de chaque fichier d'en-tête et ses fonctions les plus importantes.

---

### 1. `game.h` - Le Chef d'Orchestre

- **Rôle** : Définit la structure principale `Game` qui contient tout le jeu et déclare les fonctions qui gèrent le cycle de vie du jeu.

- **Fonctions principales** :
    - `int init_game()` : Initialise tous les composants (fenêtre, rendu, etc.).
    - `void handle_game_events()` : Gère les entrées du joueur (clavier).
    - `void update_game()` : Met à jour la logique du jeu (ex: vérifier la victoire).
    - `void render_game()` : Affiche le jeu à l'écran.

---

### 2. `world.h` - Le Plateau de Jeu

- **Rôle** : Définit la carte du jeu et contient la logique de base du Sokoban.

- **Fonctions principales** :
    - `bool move_player()` : Gère le déplacement du joueur et la mécanique de poussée des caisses.
    - `bool has_player_won()` : Vérifie si toutes les caisses sont sur leurs cibles.
    - `Position find_player_position()` : Trouve les coordonnées du joueur sur la carte.

---

### 3. `world_view.h` - L'Affichage Graphique

- **Rôle** : Déclare les fonctions responsables de dessiner le monde du jeu à l'écran.

- **Fonctions principales** :
    - `void render_view()` : Dessine la carte, les murs, les caisses et le joueur en utilisant la feuille de sprites.
    - `void randomize_theme()` : Change l'apparence des sprites pour un aspect visuel différent à chaque niveau.
    - `bool init_view()` : Charge la feuille de sprites et initialise la vue.

---

### 4. `world_parser.h` - Le Lecteur de Niveaux

- **Rôle** : Fournit les outils pour lire et analyser les fichiers de niveaux (`.txt`).

- **Fonctions principales** :
    - `bool load_level()` : Lit un fichier texte et le convertit en une carte de jeu utilisable par le module `world`.
    - `bool init_parser()` : Compte le nombre de niveaux disponibles dans un fichier.

---

### 5. `best_score.h` - La Mémoire des Scores

- **Rôle** : Déclare les fonctions pour sauvegarder et charger les meilleurs scores.

- **Fonctions principales** :
    - `void update_best_score()` : Enregistre un nouveau meilleur score pour un niveau.
    - `int get_best_score()` : Récupère le meilleur score enregistré pour un niveau.
