# Résumé du Projet Sokoban (Présentation 4 min)

Ce document résume l'architecture du projet Sokoban, un jeu de puzzle développé en C avec la bibliothèque SDL3.

## 1. Vue d'ensemble

Le projet est divisé en modules distincts, chacun ayant une responsabilité claire. Le `Makefile` permet de compiler et d'exécuter le jeu facilement avec la commande `make run`.

---

## 2. Les Fichiers d'En-tête (.h)

Les fichiers d'en-tête définissent les interfaces et les structures de données partagées entre les différents modules. Ils permettent une organisation modulaire et une meilleure maintenabilité du code.

### `best_score.h`
Déclare les fonctions pour la gestion des meilleurs scores du jeu.

### `game.h`
Définit la structure principale du jeu (`Game`) et déclare les fonctions pour la gestion globale du jeu (initialisation, événements, mise à jour, rendu).

### `world.h`
Définit la structure du monde du jeu (`World`), les constantes (murs, caisses, joueur) et les fonctions de logique de base (déplacement, victoire).

### `world_parser.h`
Déclare la structure (`WorldParser`) et les fonctions pour l'analyse des fichiers de niveaux.

### `world_view.h`
Définit la structure de la vue (`WorldView`) et déclare les fonctions pour le rendu graphique du monde du jeu.

---

## 3. Les Modules Sources (.c)

### `main.c` - Le Point d'Entrée
- C'est le point de départ de l'application.
- Son unique rôle est d'initialiser la structure principale du jeu (`Game`) et de lancer la **boucle de jeu**.
- À la fin, il s'assure de libérer la mémoire correctement.

### `game.c` - Le Cerveau du Jeu
- C'est le module central qui orchestre tout.
- Il contient la **machine à états** qui gère les différentes phases : menu principal, jeu en cours, ou fin de partie.
- Il capture les **entrées du clavier** (déplacements, redémarrer, etc.) et met à jour l'état du jeu en conséquence.
- Il fait le lien entre la logique du jeu (`world`) et l'affichage (`world_view`).

### `world.c` - La Logique du Puzzle
- Ce module représente le **monde du jeu** : la carte, la position du joueur, les murs et les caisses.
- Il gère les **règles du Sokoban** :
    - Déplacement du joueur.
    - Pousser une caisse (et seulement si l'espace derrière est libre).
    - Détecter la **condition de victoire** (toutes les caisses sur les objectifs).
- Utilise une représentation efficace de la carte (bitmasks) pour gérer les superpositions (ex: joueur sur un objectif).

### `world_view.c` - L'Affichage (Les Yeux)
- Ce module est entièrement dédié au **rendu graphique**.
- Il parcourt la carte du module `world` et dessine chaque élément (sol, murs, caisses, joueur) à l'écran.
- Il utilise une **feuille de sprites** (`sprites.png`) pour toutes les textures.
- Il gère également les animations simples, comme l'orientation du personnage.

### `world_parser.c` - Le Lecteur de Niveaux
- Sa seule tâche est de **lire les fichiers texte** (`.txt`) qui contiennent les niveaux.
- Il analyse le texte et le traduit dans un format que le module `world` peut comprendre et utiliser pour construire la carte.

### `best_score.c` - La Gestion des Scores
- Un module simple qui sauvegarde le meilleur score pour chaque niveau dans un fichier (`best_scores.txt`).
- Il permet de lire et d'écrire les scores, offrant une persistance entre les parties.

---

## 4. Enchaînement
1.  `main.c` démarre le jeu.
2.  `game.c` affiche le menu principal.
3.  Le joueur choisit une difficulté, `game.c` demande à `world_parser.c` de charger le premier niveau.
4.  À chaque tour de boucle, `game.c` attend une action du joueur.
5.  Si le joueur se déplace, `game.c` demande à `world.c` de valider et d'appliquer le mouvement.
6.  `game.c` demande ensuite à `world_view.c` de redessiner l'écran avec la nouvelle position.
7.  Ce cycle continue jusqu'à ce que `world.c` signale à `game.c` que le niveau est terminé.
