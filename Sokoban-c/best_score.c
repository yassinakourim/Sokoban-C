#include "best_score.h"
#include <stdio.h>
#include <stdlib.h>

#define BEST_SCORE_FILE "best_scores.txt"

void update_best_score(size_t level_index, int score) {
    FILE* file = fopen(BEST_SCORE_FILE, "r+");
    if (file == NULL) {
        file = fopen(BEST_SCORE_FILE, "w+");
        if (file == NULL) {
            return;
        }
    }

    size_t current_level = 0;
    int current_score = 0;
    long int level_pos = -1;

    while (fscanf(file, "%zu: %d\n", &current_level, &current_score) != EOF) {
        if (current_level == level_index) {
            level_pos = ftell(file);
            break;
        }
    }

    if (level_pos != -1) {
        fseek(file, level_pos, SEEK_SET);
        fprintf(file, "%zu: %d\n", level_index, score);
    } else {
        fseek(file, 0, SEEK_END);
        fprintf(file, "%zu: %d\n", level_index, score);
    }

    fclose(file);
}

int get_best_score(size_t level_index) {
    FILE* file = fopen(BEST_SCORE_FILE, "r");
    if (file == NULL) {
        return -1;
    }

    size_t current_level = 0;
    int current_score = 0;

    while (fscanf(file, "%zu: %d\n", &current_level, &current_score) != EOF) {
        if (current_level == level_index) {
            fclose(file);
            return current_score;
        }
    }

    fclose(file);
    return -1;
}
