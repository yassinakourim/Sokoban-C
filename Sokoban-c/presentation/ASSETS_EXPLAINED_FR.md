# Explication du Répertoire `assets` et du Fichier `sprites (2).xml`

Ce document analyse les fichiers non-code du projet : les ressources graphiques, les polices, les niveaux, ainsi qu'un fichier de métadonnées XML.

---

## 1. Le Répertoire `assets`

Le répertoire `assets` contient toutes les ressources externes (les "assets") nécessaires au fonctionnement du jeu. Il est organisé en sous-répertoires par type de ressource.

*   **`assets/fonts/font.ttf`**
    *   **Rôle** : C'est un fichier de police de caractères (TrueType Font). Il est chargé par la bibliothèque `SDL_ttf` au démarrage du jeu et est utilisé pour dessiner tout le texte visible à l'écran, comme le compteur de pas, le numéro du niveau et les instructions de contrôle.

*   **`assets/images/menu/`**
    *   **Rôle** : Contient les images utilisées dans le menu principal. Les fichiers `easy.png` et `hard.png` sont chargés pour servir de boutons de sélection de la difficulté.

*   **`assets/images/spritesheet/sprites.png`**
    *   **Rôle** : C'est l'atout graphique le plus important du jeu. Il s'agit d'une "feuille de sprites" (ou "texture atlas"), c'est-à-dire une unique grande image qui rassemble tous les petits éléments graphiques du jeu : les murs, les sols, les caisses, les objectifs et toutes les images d'animation du personnage.
    *   **Avantage** : Cette technique est très efficace. Au lieu de charger des dizaines de petits fichiers, la carte graphique ne charge qu'une seule texture, ce qui optimise grandement les opérations de rendu.

*   **`assets/levels/`**
    *   **Rôle** : Contient les fichiers de données des niveaux. `easy_levels.txt` et `hard_levels.txt` sont de simples fichiers texte où les niveaux sont "dessinés" avec des caractères (`#` pour un mur, `@` pour le joueur, etc.). Ces fichiers sont lus et interprétés par le module `world_parser`.

---

## 2. Le Fichier `sprites (2).xml`

*   **À quoi sert-il ?**
    Ce fichier est un fichier de **métadonnées** qui décrit le contenu de l'image `sprites.png`. C'est une définition d'atlas de texture. Il agit comme une "carte" ou un "index" de la feuille de sprites.

*   **Comment est-il structuré ?**
    Le fichier contient une liste d'éléments `<SubTexture>`. Chacun correspond à un sprite unique à l'intérieur de la grande image et possède des attributs :
    *   `name` : Le nom du sprite (par ex. `Crate_Blue.png`).
    *   `x`, `y` : Les coordonnées en pixels du coin supérieur gauche du sprite dans `sprites.png`.
    *   `width`, `height` : La largeur et la hauteur de ce sprite.

*   **Quel est son rôle DANS CE projet ?**
    C'est un point d'analyse crucial : **le code du jeu en C ne lit et n'utilise pas directement ce fichier XML**.

    Alors, pourquoi existe-t-il ? Il s'agit très probablement d'un artefact du processus de développement. Le développeur a probablement utilisé un outil (comme TexturePacker) pour prendre de nombreuses images individuelles et les combiner automatiquement en une seule feuille de sprites, `sprites.png`. Cet outil a aussi généré ce fichier XML pour décrire où chaque image a été placée.

    Ensuite, le développeur a **manuellement copié** les coordonnées (`x`, `y`, `width`, `height`) de ce fichier XML dans le code C, plus précisément dans le fichier `world_view.h`, pour créer les constantes `static const SDL_FRect` (par exemple `RECT_WALLS`, `RECT_CHAR_1`, etc.).

    En conclusion, bien que non utilisé directement par le programme final, ce fichier XML reste une documentation très utile et lisible par un humain pour comprendre la composition de la feuille de sprites `sprites.png`.
