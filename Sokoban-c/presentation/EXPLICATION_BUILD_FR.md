# Explication du Processus de Build du Projet Sokoban

Ce document explique le rôle, le fonctionnement et l'importance du `Makefile` dans ce projet, ainsi que les considérations pour l'exécuter sur Windows.

## 1. Rôle et Importance du `Makefile`

Le `Makefile` est un script qui automatise entièrement le processus de construction (build) du projet. Son rôle principal est de compiler tous les fichiers de code source (`.c`) et de les lier avec les bibliothèques `SDL3` requises pour créer un unique fichier exécutable nommé `sokoban`.

Son importance réside dans plusieurs points :

*   **Automatisation :** Au lieu de taper manuellement des commandes de compilation complexes pour chaque fichier source, il suffit d'exécuter la commande `make`.
*   **Efficacité :** `make` est intelligent. Il ne recompile que les fichiers qui ont été modifiés depuis la dernière compilation, ce qui permet de gagner beaucoup de temps.
*   **Gestion des Dépendances :** Le projet dépend de bibliothèques externes (`SDL3`, `SDL3_image`, `SDL3_ttf`). Le `Makefile` utilise l'outil `pkg-config` pour trouver et inclure automatiquement les bons chemins et options pour ces bibliothèques, ce qui rend la compilation fiable.
*   **Standardisation :** `make` est un outil standard dans le monde du développement logiciel, en particulier sur Linux et macOS. Le `Makefile` sert de documentation universelle sur la manière de construire le projet.

## 2. Comment fonctionne le `Makefile`

Le `Makefile` utilise un ensemble de règles et de variables pour accomplir sa tâche.

*   **Variables :** Il définit des variables pour le compilateur (`CC = gcc`), les fichiers source (`SRCS`), et les fichiers objets (`OBJS`).
*   **Flags :** Il configure les options du compilateur (`CFLAGS`) pour les avertissements et la version du langage C, et les options de l'éditeur de liens (`LDFLAGS`) pour lier les bibliothèques. La commande `$(shell pkg-config ...)` exécute `pkg-config` pour obtenir les chemins spécifiques aux bibliothèques SDL sur le système.
*   **Cibles (Targets) :** Il définit des "cibles" que `make` peut construire :
    *   `all` : La cible par défaut. Elle construit l'exécutable `sokoban`.
    *   `run` : Une cible pratique qui compile d'abord le projet (en appelant `all`) puis l'exécute immédiatement.
    *   `clean` : Cette cible supprime tous les fichiers générés par la compilation, nettoyant ainsi le répertoire.

## 3. Compatibilité avec Windows

Le projet **ne peut pas** être compilé et exécuté "tel quel" sur Windows en utilisant la même méthode que sur Linux.

La raison est que le processus de construction dépend d'outils comme `gcc`, `make`, et `pkg-config`, qui ne sont pas installés par défaut sur Windows.

Pour faire fonctionner ce projet sur Windows, il existe deux approches principales :

1.  **Environnement de Compatibilité (Recommandé) :** Installer un environnement de type Unix pour Windows comme **MSYS2** ou **MinGW**. Ces outils fournissent `gcc`, `make`, et un gestionnaire de paquets pour installer facilement les bibliothèques SDL. Une fois cet environnement configuré, vous pouvez utiliser le `Makefile` original sans aucune modification.
2.  **IDE et Compilateur Natif :** Utiliser un compilateur C/C++ natif et un IDE comme **Visual Studio**. Dans ce cas, vous n'utiliserez pas le `Makefile`. Vous devriez créer un nouveau projet, y ajouter les fichiers `.c`, et configurer manuellement le projet pour qu'il trouve les en-têtes (`.h`) et les bibliothèques (`.lib`) de SDL que vous auriez téléchargées séparément. Il faudrait également copier les fichiers `.dll` de SDL à côté de votre fichier `.exe` final.
