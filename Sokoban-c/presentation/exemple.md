# Présentation du Projet Sokoban en Langage C  
**Durée : 15 minutes · Public : 5 personnes**

---

## I. Introduction (2 min)

### Description générale du jeu  
Sokoban est un jeu de réflexion en 2D où le joueur déplace des caisses vers des objectifs dans un labyrinthe. Chaque niveau est un puzzle qui demande une planification stratégique.

### Règles du jeu implémentées  
- Le joueur ne peut pousser qu’une seule caisse à la fois.  
- Les caisses ne peuvent pas être tirées.  
- Les murs bloquent le mouvement.  
- Le niveau est terminé lorsque toutes les caisses sont placées sur les objectifs.  
- Un compteur de pas est affiché pour chaque niveau.  
- Meilleurs scores sauvegardés par niveau.  

---

## II. Conception du jeu (6 min)

### Structure du programme (arborescence)  
```
sokoban/
├── assets/                 # Ressources graphiques et données
│   ├── fonts/             # Police de caractères
│   ├── images/            # Images du menu et spritesheet
│   ├── levels/            # Fichiers des niveaux (easy/hard)
│   └── sprites (2).xml   # Métadonnées de la spritesheet (non utilisé en runtime)
├── src/                   # Code source
│   ├── main.c            # Point d'entrée et boucle principale
│   ├── game.h/.c         # Gestion de l'état et machine à états
│   ├── world.h/.c        # Logique du monde et mouvement
│   ├── world_view.h/.c   # Rendu graphique et thèmes
│   ├── world_parser.h/.c # Lecture des fichiers de niveaux
│   ├── best_score.h/.c   # Gestion des meilleurs scores
│   └── Makefile          # Automatisation de la compilation
```

### Librairies standards utilisées  
- **SDL3** : Gestion de la fenêtre, rendu, événements.  
- **SDL3_ttf** : Affichage de texte.  
- **SDL3_image** : Chargement d'images.  
- **Standard C** : `stdio.h`, `stdlib.h`, `time.h`, `stdint.h`.

### Structures de données définies  
- **`World`** : Carte du jeu avec masques de bits.  
- **`Game`** : État complet du jeu (monde, rendu, menu, scores).  
- **`Position`** / **`Direction`** : Coordonnées et orientation.  
- **`GameState`** / **`Difficulty`** : Machine à états et difficulté.  

### Headers et modules principaux  

| Fichier | Rôle | Fonctions clés |
|---------|------|----------------|
| `game.h/.c` | Cœur du jeu, machine à états | `init_game`, `handle_game_events`, `update_game`, `render_game` |
| `world.h/.c` | Logique du monde et mouvement | `move_player`, `has_player_won`, `find_player_position` |
| `world_view.h/.c` | Rendu graphique et thèmes | `init_view`, `render_view`, `randomize_theme` |
| `world_parser.h/.c` | Lecture des niveaux | `init_parser`, `load_level` |
| `best_score.h/.c` | Gestion des scores | `get_best_score`, `update_best_score` |

---

## III. Réalisation (5 min)

### Maquettes des écrans du jeu  
1. **Écran d’accueil** : Menu avec choix de difficulté (Easy / Hard).  
2. **Écran de jeu** :  
   - Grille du niveau avec murs, caisses, objectifs, joueur.  
   - HUD : pas, meilleur score, nom du niveau.  
3. **Écran de fin** : Message de victoire après le dernier niveau.  

### Fichiers utilisés pour le stockage des données  
- **`assets/levels/easy_levels.txt`** et **`hard_levels.txt`** : Niveaux au format texte.  
- **`best_scores.txt`** : Meilleurs scores par niveau (format `index:score`).  
- **`sprites.png`** : Feuille de sprites unique pour toutes les textures.  

### Démonstration d’une partie du jeu  
- Lancement du jeu via `make run`.  
- Navigation dans le menu avec les flèches.  
- Déplacement du joueur, poussée des caisses.  
- Changement de thème visuel avec la touche `T`.  
- Passage au niveau suivant / précédent.  
- Affichage du meilleur score en temps réel.  

---

## IV. Conclusion (2 min)

### Points forts du projet  
- **Architecture modulaire** : Séparation claire entre logique, rendu, données.  
- **Utilisation de SDL3** : Interface graphique fluide et portable.  
- **Gestion des scores persistants** : Fichier texte simple et efficace.  
- **Feuille de sprites unique** : Optimisation des performances de rendu.  

### Améliorations possibles  
- Interface graphique plus riche (animations, sons).  
- Système de sauvegarde automatique.  
- Éditeur de niveaux intégré.  
- Support multijoueur ou mode contre-la-montre.  

### Compétences mises en œuvre  
- Programmation en C avec structures et pointeurs.  
- Gestion de mémoire et ressources SDL.  
- Conception de machines à états.  
- Lecture/écriture de fichiers.  
- Travail avec un système de build (`Makefile`).  

---

**Fin de la présentation · 

