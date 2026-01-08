# Explication des Fichiers `world_parser.h` et `world_parser.c`

Ces deux fichiers forment le module d'analyse (`parser`). Leur unique responsabilité est de lire un fichier texte (`.txt`) contenant un ou plusieurs niveaux de Sokoban, et de traduire cette représentation textuelle en une structure de données `World` que le reste du jeu peut comprendre et utiliser.

---

## `world_parser.h` : Le Fichier d'En-tête

Ce fichier définit la structure de données et les fonctions publiques pour l'analyseur de niveaux.

### Structure de Données `WorldParser`

La structure `WorldParser` contient l'état de l'analyseur :

*   `const char* level_file_path` : Le chemin vers le fichier `.txt` contenant les niveaux.
*   `size_t number_of_levels` : Le nombre total de niveaux détectés dans le fichier.
*   `size_t current_level` : L'index du niveau qui doit être chargé lors du prochain appel à `load_level`.
*   `char current_level_name[...]` : Un tableau de caractères pour stocker le nom du niveau en cours de lecture.

### Déclarations de Fonctions

*   **`init_parser(...)`** : Prépare l'analyseur en lisant un fichier de niveaux pour la première fois afin de compter le nombre total de niveaux.
*   **`load_level(...)`** : Lit un niveau spécifique du fichier et remplit une structure `World` avec les données de la carte.
*   **`print_world(...)`** : Une fonction de débogage (dont l'implémentation est vide dans `world_parser.c`).

---

## `world_parser.c` : L'Implémentation de la Logique d'Analyse

Ce fichier contient la logique pour lire et interpréter le fichier de niveaux.

### Le Format des Fichiers de Niveaux

L'analyseur s'attend à un format de fichier texte spécifique :
*   Les niveaux sont séparés par une ligne contenant uniquement le caractère `~`.
*   La ligne qui suit immédiatement un `~` est considérée comme le nom du niveau.
*   Le reste des lignes jusqu'au prochain `~` (ou la fin du fichier) constitue la carte du niveau.
*   Les caractères suivants sont utilisés pour dessiner la carte :
    *   ` ` (espace) : Sol vide
    *   `#` : Mur
    *   `@` : Joueur
    *   `$` : Caisse
    *   `.` : Objectif
    *   `*` : Une caisse sur un objectif
    *   `+` : Le joueur sur un objectif

### Analyse des Fonctions Clés

*   **`txt_char_to_map_byte(char txt_char)`** : C'est une fonction de traduction cruciale. Elle prend un caractère du fichier (par ex. `'@'`) et le convertit en sa valeur de masque de bits correspondante (`FLAG_PLAYER`). Une instruction `switch` est utilisée pour gérer toutes les conversions.

*   **`init_parser(WorldParser* parser, ...)`** : Cette fonction effectue un **premier passage** sur le fichier de niveaux.
    1.  Elle ouvre le fichier.
    2.  Elle lit le fichier ligne par ligne dans le seul but de compter le nombre de fois où une ligne commence par `~`.
    3.  Ce décompte lui permet de connaître le nombre total de niveaux (`number_of_levels`) dans le fichier.
    4.  Elle referme ensuite le fichier.

*   **`load_level(WorldParser* parser, World* world)`** : Cette fonction effectue le **deuxième passage** et le travail principal.
    1.  Elle **rouvre** le fichier à chaque appel.
    2.  Elle "saute" les niveaux précédents en lisant le fichier depuis le début et en comptant les séparateurs `~` jusqu'à atteindre l'index du `current_level` à charger.
    3.  Une fois au bon endroit, elle lit le nom du niveau.
    4.  Ensuite, elle lit la carte, ligne par ligne, jusqu'à rencontrer le prochain `~` ou la fin du fichier.
    5.  Pour chaque ligne de la carte, elle parcourt chaque caractère.
    6.  Pour chaque caractère, elle appelle `txt_char_to_map_byte` pour obtenir la valeur numérique (le masque de bits) et la stocke dans la grille `world->map`.
    7.  Pendant cette lecture, elle calcule également la largeur (`width`) et la hauteur (`height`) maximales du niveau.
    8.  Une fois la lecture du niveau terminée, elle referme le fichier.

Cette approche, bien que peu performante car elle rouvre le fichier à chaque fois, est simple et robuste pour des fichiers de niveaux de taille raisonnable.
