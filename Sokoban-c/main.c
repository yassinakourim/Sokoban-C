#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
  // Initialise le générateur de nombres aléatoires, utile pour des aspects du jeu qui nécessitent de l'aléa.
  srand(time(NULL));

  // Déclare une variable 'game' de type Game, qui contiendra toutes les informations sur l'état du jeu.
  Game game;

  // Initialise la structure du jeu. Si l'initialisation échoue (par exemple, ne peut pas allouer de la mémoire), le programme se termine.
  if (init_game(&game) != 0) {
    return 1; // Quitte avec un code d'erreur.
  }

  // C'est la boucle de jeu principale. Tant que 'is_running' est vrai, le jeu continue.
  while (game.is_running) {
    // Gère les entrées de l'utilisateur (clavier, souris, etc.).
    handle_game_events(&game);
    // Met à jour l'état du jeu (position des personnages, logique du jeu, etc.).
    update_game(&game);
    // Dessine le jeu à l'écran.
    render_game(&game);
  }

  // Libère toutes les ressources allouées pour le jeu avant de quitter.
  clean_game(&game);

  return 0; // Le programme s'est terminé avec succès.
}