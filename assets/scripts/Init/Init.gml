// set grid size 
global.grid_size = [64, 64];	// grid size [x, y]

// game states
enum game_state {
	starting,
	running,
	victory,
	defeat,
}

// current level of the player - used to show message 
global.level = 0;

// current score
global.time = 0;	// amount of time
global.movements = 0;	// amout of steps

// load highscores
ini_open("savedata.ini");
global.besttime = ini_read_real("save1", "time", 0 );
global.bestmovements = ini_read_real("save1", "movements", 0 );
ini_close();