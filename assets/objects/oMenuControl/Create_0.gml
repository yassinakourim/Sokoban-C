/// @description Start Variables

// start music
audio_stop_all();
audio_play_sound(sndMusic, 100, true);

// reset level
global.level = 0;

// reset current score
global.time = 0;
global.movements = 0;

// change depth to be above all else
depth = -1000;

// create the list for the menu
menu = ds_list_create();
ds_list_add(menu, "Start Game", "Credits", "Exit Game");

// menu selected
selected = 0;

// menu variables
menu_start_position = 250;	// start y position of the menu
menu_separation = 60;		// y sepraration between menu items
angle = 0;	// used for arrow side animation
delta_x = 10;	// maximum x movement for the arrow animation