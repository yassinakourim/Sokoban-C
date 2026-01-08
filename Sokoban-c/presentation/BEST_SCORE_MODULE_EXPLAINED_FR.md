# Explication des Fichiers `best_score.h` et `best_score.c`

Ce dernier module est responsable d'une fonctionnalité simple mais importante : la persistance des meilleurs scores. Il gère la lecture et l'écriture des meilleurs scores pour chaque niveau dans un fichier texte.

---

## `best_score.h` : Le Fichier d'En-tête

Le fichier d'en-tête est très simple et déclare les deux fonctions publiques du module :

*   **`int get_best_score(size_t level_index)`** : Récupère le meilleur score pour un niveau donné.
*   **`void update_best_score(size_t level_index, int score)`** : Met à jour (ou crée) le meilleur score pour un niveau donné.

---

## `best_score.c` : L'Implémentation de la Gestion des Scores

Ce fichier implémente la logique de lecture et d'écriture dans le fichier `best_scores.txt`.

### Format du Fichier `best_scores.txt`

Le module s'attend à un format simple où chaque ligne représente le score d'un niveau :
```
<index_du_niveau>: <score>
```
Par exemple :
```
0: 42
1: 108
2: 95
```

### Analyse des Fonctions Clés

*   **`get_best_score(size_t level_index)`** :
    1.  Ouvre `best_scores.txt` en mode lecture (`"r"`). S'il n'existe pas, la fonction retourne `-1` (pas de score).
    2.  Lit le fichier ligne par ligne avec `fscanf`, en extrayant l'index du niveau et le score de chaque ligne.
    3.  Dès qu'elle trouve une ligne correspondant au `level_index` demandé, elle retourne immédiatement le score trouvé.
    4.  Si la fin du fichier est atteinte sans trouver de correspondance, elle retourne `-1`.

*   **`update_best_score(size_t level_index, int score)`** :
    Cette fonction est plus complexe car elle tente de modifier le fichier "sur place" au lieu de le réécrire entièrement.
    1.  Elle tente d'ouvrir le fichier en mode lecture/écriture (`"r+"`). Si cela échoue (le fichier n'existe pas), elle le crée en l'ouvrant en mode écriture/lecture (`"w+"`).
    2.  Elle parcourt le fichier pour voir si un score existe déjà pour le `level_index` donné.
    3.  **Cas 1 : Un score existe déjà.** La fonction mémorise la position dans le fichier, puis utilise `fseek` pour revenir à cette position et `fprintf` pour "écraser" l'ancienne valeur avec le nouveau score.
        *   **Note technique importante :** Cette méthode présente un **bug potentiel**. Elle ne fonctionne de manière fiable que si le nouveau score a exactement le même nombre de chiffres que l'ancien (par exemple, remplacer 105 par 102). Si on remplace 99 (2 chiffres) par 100 (3 chiffres), le fichier risque d'être corrompu car le nouveau texte est plus long que l'ancien.
    4.  **Cas 2 : Aucun score n'existe pour ce niveau.** La fonction va à la toute fin du fichier (`fseek(..., SEEK_END)`) et ajoute une nouvelle ligne avec le niveau et le score.
    5.  Enfin, elle ferme le fichier.

Cette approche de modification "sur place" est une tentative d'optimisation pour éviter de réécrire tout le fichier, mais elle introduit une certaine fragilité. Pour un petit fichier de scores, une approche plus sûre serait de tout lire en mémoire, de modifier les données en mémoire, puis de réécrire le fichier en entier.
