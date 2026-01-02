#pragma once
#include "entity.h"
#include <SDL3/SDL.h>

// This allows main.c to create crates
Entity init_crate(SDL_Renderer* renderer, float x, float y);

// This allows player.c to check if it can push a crate
bool try_push_crate(float target_x, float target_y, float dx, float dy);