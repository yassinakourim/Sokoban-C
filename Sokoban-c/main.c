#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
  srand(time(NULL));
  Game game;

  if (init_game(&game) != 0) {
    return 1;
  }

  while (game.is_running) {
    handle_game_events(&game);
    update_game(&game);
    render_game(&game);
  }

  clean_game(&game);

  return 0;
}