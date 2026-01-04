#pragma once
#include "world.h"

#define MAX_LINE_SIZE WORLD_MAX_DIM

typedef struct {
  const char* level_file_path;
  size_t number_of_levels;
  size_t current_level;
  char current_level_name[MAX_LINE_SIZE];
} WorldParser;

bool init_parser(WorldParser* parser, const char* level_file_path);
bool load_level(WorldParser* parser, World* world);
void print_world(World* world);