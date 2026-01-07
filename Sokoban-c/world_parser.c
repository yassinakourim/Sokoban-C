#include "world_parser.h"
#include <stdio.h>
#include <string.h>

#define SYMB_EMPTY ' '
#define SYMB_WALL '#'
#define SYMB_CRATE '$'
#define SYMB_GOAL '.'
#define SYMB_CRATE_ON_GOAL '*'
#define SYMB_PLAYER '@'
#define SYMB_PLAYER_ON_GOAL '+'

char map_byte_to_txt_char(uint8_t map_byte) {
  if (map_byte == FLAG_WALL) return SYMB_WALL;
  if (map_byte == FLAG_CRATE) return SYMB_CRATE;
  if (map_byte == FLAG_GOAL) return SYMB_GOAL;
  if (map_byte == FLAG_GOAL + FLAG_CRATE) return SYMB_CRATE_ON_GOAL;
  if (map_byte == FLAG_PLAYER) return SYMB_PLAYER;
  if (map_byte == FLAG_GOAL + FLAG_PLAYER) return SYMB_PLAYER_ON_GOAL;
  return SYMB_EMPTY;
}

uint8_t txt_char_to_map_byte(char txt_char) {
  switch (txt_char) {
    case SYMB_WALL:
      return FLAG_WALL;
    case SYMB_CRATE:
      return FLAG_CRATE;
    case SYMB_GOAL:
      return FLAG_GOAL;
    case SYMB_CRATE_ON_GOAL:
      return FLAG_GOAL + FLAG_CRATE;
    case SYMB_PLAYER:
      return FLAG_PLAYER;
    case SYMB_PLAYER_ON_GOAL:
      return FLAG_GOAL + FLAG_PLAYER;
    default:
      return EMPTY;
  }
}

bool init_parser(WorldParser* parser, const char* level_file_path) {
  memset(parser, 0, sizeof(*parser));
  parser->level_file_path = level_file_path;

  FILE* level_file = fopen(parser->level_file_path, "r");
  if (level_file == NULL) return false;

  char file_line[MAX_LINE_SIZE];
  bool file_was_empty = true;

  while (fgets(file_line, MAX_LINE_SIZE, level_file)) {
    if (file_line[0] == '~') {
      parser->number_of_levels += 1;
    }
    file_was_empty = false;
  }

  if (!file_was_empty) {
    parser->number_of_levels += 1;
  }

  fclose(level_file);
  return parser->number_of_levels != 0;
}

bool load_level(WorldParser* parser, World* world) {
  
  FILE* level_file = fopen(parser->level_file_path, "r");
  if (level_file == NULL) {
    
    return false;
  }
  
  memset(world->map, 0, sizeof(world->map));
  world->height = 0;
  world->width = 0;

  char file_line[MAX_LINE_SIZE];
  size_t level_being_read = 0;

  while (fgets(file_line, MAX_LINE_SIZE, level_file)) {
    if (level_being_read == parser->current_level) {
      break;
    }

    if (file_line[0] == '~') {
      level_being_read += 1;
    }
  }

  file_line[strcspn(file_line, "\n")] = 0;  // Removing trailing '\n'
  strcpy(parser->current_level_name, file_line);

  size_t level_width, level_height;
  level_width = 0;
  level_height = 0;

  while (fgets(file_line, MAX_LINE_SIZE, level_file)) {
    if (file_line[0] == '~') {
      break;
    }

    file_line[strcspn(file_line, "\n")] = 0;  // Removing trailing '\n'

    size_t line_width = strlen(file_line);

    level_height += 1;
    level_width = (line_width > level_width) ? line_width : level_width;

    if (level_height >= WORLD_MAX_DIM || level_width >= WORLD_MAX_DIM)
      return false;

    for (size_t i_char = 0; i_char < line_width; i_char += 1) {
      world->map[level_height - 1][i_char] =
          txt_char_to_map_byte(file_line[i_char]);
    }
  }

  world->height = level_height;
  world->width = level_width;

  return true;
}

void print_world(World* world) {
  for (size_t line = 0; line < world->height; line += 1) {
    for (size_t col = 0; col < world->width; col += 1) {
      
    }
    
  }
}