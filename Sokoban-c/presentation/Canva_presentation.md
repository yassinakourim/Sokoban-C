# Présentation du Projet Sokoban

---

## Slide 1: Titre

**Sokoban**

Un jeu de puzzle classique implémenté en C avec SDL3.

---

## Slide 2: I. Introduction - Description Générale

*   **Qu'est-ce que Sokoban ?**
    *   Un jeu de puzzle où le joueur doit pousser des caisses pour les amener sur des zones de stockage (objectifs).
*   **Objectif du Projet**
    *   Implémenter une version fonctionnelle et complète de Sokoban en langage C.
    *   Utiliser la bibliothèque SDL3 pour la gestion graphique et les interactions.
    *   Créer une architecture modulaire et bien structurée.

---

## Slide 3: I. Introduction - Règles du Jeu Implémentées

*   Le joueur peut se déplacer dans les 4 directions (haut, bas, gauche, droite).
*   Le joueur peut pousser une seule caisse à la fois.
*   Il est impossible de pousser une caisse si elle est bloquée par un mur ou une autre caisse.
*   Il est impossible de tirer une caisse.
*   Le niveau est gagné lorsque toutes les caisses sont placées sur les objectifs.

---

## Slide 4: II. Conception - Structure du Programme (Arborescence)

Le projet est organisé en modules clairs et indépendants :

*   **`main.c`**: Point d'entrée, initialise le jeu et lance la boucle principale.
*   **`game.c/.h`**: Cœur du jeu, gère les états (menu, jeu), les événements et la logique globale.
*   **`world.c/.h`**: Gère la représentation de la carte (murs, caisses, joueur) et la logique de mouvement.
*   **`world_view.c/.h`**: Responsable du rendu graphique de la carte et des objets.
*   **`world_parser.c/.h`**: Lit et interprète les fichiers de niveaux.
*   **`best_score.c/.h`**: Gère la lecture et l'écriture des meilleurs scores.
*   **`assets/`**: Contient toutes les ressources (images, polices, niveaux).

---

## Slide 5: II. Conception - Librairies Utilisées

*   **Bibliothèques Standards C**
    *   `stdlib.h`: Fonctions utilitaires générales.
    *   `time.h`: Initialisation du générateur de nombres aléatoires.
*   **Bibliothèques Externes (via SDL3)**
    *   **SDL3**: Utilisée pour la création de la fenêtre, le rendu 2D et la gestion des événements (clavier).
    *   **SDL3_image**: Utilisée pour charger les images (notamment la feuille de sprites `sprites.png`).
    *   **SDL3_ttf**: Utilisée pour le rendu de texte à l'écran (scores, pas, etc.).

---

## Slide 6: II. Conception - Structures de Données Clés

*   **`Game`**: La structure centrale qui contient l'état complet du jeu (pointeurs SDL, état actuel, monde, vue, etc.).
*   **`World`**: Représente le niveau actuel. Contient un tableau 2D `map[100][100]` où chaque case est un **masque de bits (bitmask)**.
*   **Le Masque de Bits (`uint8_t`)**
    *   Technique efficace pour superposer des objets dans une même case.
    *   `FLAG_WALL`, `FLAG_GOAL`, `FLAG_CRATE`, `FLAG_PLAYER` sont des puissances de 2.
    *   Exemple: une caisse sur un objectif = `FLAG_CRATE | FLAG_GOAL`.

---

## Slide 7: II. Conception - Headers (`.h`)

Chaque module a un fichier `.h` qui agit comme une interface publique :

*   **`game.h`**: Définit les états du jeu (`GameState`), la structure `Game` et déclare les fonctions principales (`init_game`, `render_game`...).
*   **`world.h`**: Définit les masques de bits (`FLAG_*`), la structure `World` et les fonctions de manipulation (`move_player`, `has_player_won`).
*   **`world_view.h`**: Définit les `SDL_FRect` pour la feuille de sprites et la structure `WorldView`.
*   **`world_parser.h`**: Définit la structure `WorldParser` et les fonctions de chargement de niveaux.
*   **`best_score.h`**: Déclare les fonctions `get_best_score` et `update_best_score`.

---

## Slide 8: II. Conception - Modules (`.c`)

Chaque module `.c` implémente une responsabilité unique :

*   **`game.c`**: Le chef d'orchestre. Contient une machine à états (`switch (game->state)`) qui délègue le travail aux bons modules en fonction de l'état (Menu Principal ou Jeu).
*   **`world.c`**: La "physique" du jeu. Implémente la logique de déplacement, de poussée et de victoire.
*   **`world_view.c`**: Le "peintre". Parcourt la carte du `World` et dessine chaque élément à l'écran en utilisant une feuille de sprites pour l'efficacité.
*   **`world_parser.c`**: L'"interprète". Lit les fichiers `.txt`, traduit les caractères (`#`, `@`, `$`) en masques de bits et remplit la structure `World`.
*   **`best_score.c`**: Le "gardien des scores". Lit et écrit dans `best_scores.txt`.

---

## Slide 9: III. Réalisation - Maquettes des Écrans

Le jeu possède deux écrans principaux :

*   **Menu Principal**
    *   Affiche le titre du jeu.
    *   Permet de choisir la difficulté ("Easy" ou "Hard").
    *   La sélection se fait avec les flèches du clavier.
*   **Écran de Jeu**
    *   Affiche le niveau (murs, sol, caisses, objectifs, joueur).
    *   Affiche une interface (HUD) avec des informations textuelles :
        *   Nom du niveau
        *   Numéro du niveau actuel / Nombre total de niveaux
        *   Compteur de pas
        *   Meilleur score pour ce niveau

---

## Slide 10: III. Réalisation - Stockage des Données

Le jeu utilise des fichiers texte simples pour la persistance :

*   **Fichiers de Niveaux (`easy_levels.txt`, `hard_levels.txt`)**
    *   Les niveaux sont "dessinés" avec des caractères ASCII.
    *   Un caractère `~` sépare les niveaux.
    *   Le nom du niveau suit la ligne du `~`.
*   **Fichier des Meilleurs Scores (`best_scores.txt`)**
    *   Stocke le meilleur score pour chaque niveau.
    *   Format : `<index_du_niveau>: <score>` par ligne.
    *   *Note: L'implémentation de la mise à jour a un bug potentiel si la taille du score change (ex: de 99 à 100).*

---

## Slide 11: III. Réalisation - Démonstration (Boucle de Jeu)

La boucle de jeu principale dans `main.c` est simple et classique :

```c
while (game.is_running) {
  // 1. Gestion des entrées
  handle_game_events(&game);

  // 2. Mise à jour de la logique
  update_game(&game);

  // 3. Rendu graphique
  render_game(&game);
}
```
*   `handle_game_events`: Récupère les pressions de touches (déplacement, redémarrage, changement de thème).
*   `update_game`: Appelle `move_player` si nécessaire, vérifie la condition de victoire (`has_player_won`).
*   `render_game`: Appelle `render_view` pour dessiner la carte et `render_hud` pour le texte.

---

## Slide 12: IV. Conclusion

*   **Points Forts du Projet**
    *   **Architecture Modulaire Forte**: Séparation claire des responsabilités (logique, rendu, parsage).
    *   **Utilisation Efficace des Ressources**:
        *   **Masques de bits** pour une représentation mémoire compacte de la carte.
        *   **Feuille de sprites (Spritesheet)** pour un rendu graphique optimisé (un seul chargement de texture).
    *   **Code Robuste et Propre**: Utilisation de constantes, d'énumérations et de structures bien définies.

*   **Axes d'Amélioration Possibles**
    *   Ajouter des animations plus fluides ou des effets sonores.
    *   Créer un éditeur de niveaux intégré au jeu.

---
