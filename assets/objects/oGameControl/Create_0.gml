/// @description Start Variables

// add to level of the room
global.level ++;

// set depth so the draw event will be on top of everything else.
// yes, I could use the DRAW GUI event, but in games like this, I like to use this approach in simples games
depth = -1000;

// variable to hold the number of missplaced crates
number_of_missing_crates = 0;

// start game state
state = game_state.starting;
