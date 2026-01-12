#include "best_score.h"
#include <stdio.h>
#include <stdlib.h>

// Le nom du fichier où les meilleurs scores sont sauvegardés.
#define BEST_SCORE_FILE "best_scores.txt"

// Met à jour le meilleur score pour un niveau donné dans le fichier.
void update_best_score(size_t level_index, int score) {
    // Ouvre le fichier en mode lecture/écriture, le crée s'il n'existe pas.
    FILE* file = fopen(BEST_SCORE_FILE, "r+");
    if (file == NULL) {
        file = fopen(BEST_SCORE_FILE, "w+");
        if (file == NULL) {
            perror("Error opening best score file");
            return;
        }
    }

    long int pos = -1;
    size_t temp_level;
    int temp_score;

    // Cherche si un score existe déjà pour ce niveau.
    long int current_pos = ftell(file);
    while (fscanf(file, "%zu: %d\n", &temp_level, &temp_score) == 2) {
        if (temp_level == level_index) {
            pos = current_pos;
            break;
        }
        current_pos = ftell(file);
    }

    // Si un score existe et que le nouveau est meilleur, ou s'il n'y avait pas de score
    if (pos != -1) {
        // Retourne au début de la ligne du score existant pour l'écraser.
        fseek(file, pos, SEEK_SET);
        fprintf(file, "%zu: %d\n", level_index, score);
    } else {
        // Ajoute le nouveau score à la fin du fichier.
        fseek(file, 0, SEEK_END);
        fprintf(file, "%zu: %d\n", level_index, score);
    }

    fclose(file);
}

// Récupère le meilleur score pour un niveau donné depuis le fichier.
int get_best_score(size_t level_index) {
    FILE* file = fopen(BEST_SCORE_FILE, "r");
    if (file == NULL) {
        return -1; // Pas de fichier de scores, donc pas de meilleur score.
    }

    size_t current_level;
    int current_score;

    // Lit le fichier ligne par ligne pour trouver le score du niveau demandé.
    while (fscanf(file, "%zu: %d\n", &current_level, &current_score) == 2) {
        if (current_level == level_index) {
            fclose(file);
            return current_score;
        }
    }

    fclose(file);
    return -1; // Pas de score trouvé pour ce niveau.
}
