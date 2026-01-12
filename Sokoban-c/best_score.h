#pragma once

#include <stddef.h>

// Met à jour le meilleur score pour un niveau donné.
void update_best_score(size_t level_index, int score);

// Récupère le meilleur score pour un niveau donné.
int get_best_score(size_t level_index);
