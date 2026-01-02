/// @description Actions

// increase time for score
if state != game_state.victory {
	global.time += 1/60;
}

// game states
switch(state) {
	case game_state.starting:
		// if any keyboard key if pressed, set game state to running and play hurry audio
		if keyboard_check_pressed(vk_anykey) {
			state = game_state.running;	
			audio_play_sound(sndHurryUp, 100, false);
		}
		break;
	case game_state.running:
		#region
		number_of_missing_crates = 0;
		// calculating the number of missplaced crates
		// the method I used is simple, just checking if the correct crate is on top of the crate placement object
		with(oCratePlaceParent) {
			if !place_meeting(x, y, crate) { 
				other.number_of_missing_crates ++;	
			} else {
				var _crate = instance_place(x, y, crate);	
				if _crate.speed != 0 {
					other.number_of_missing_crates ++;	
				}
			}
		}
		// if therer are no more missplaced crates, the level is finished
		if number_of_missing_crates = 0 {
			oPlayer.active = false;	// this is to avoid player movement after finishing the level
			if oPlayer.speed = 0 {	// check if the player if standing still
				state = game_state.victory;		// set game state to victory
				audio_play_sound(sndVictory, 100, false);	// play level completion sound
			}
		}
		// the escape key function to restart the room
		if keyboard_check_pressed(vk_escape) {
			state = game_state.defeat;	// set game state to defeat
			audio_play_sound(sndDefeat, 100, false);	// play defeat sound
		}
		break;
		#endregion
	case game_state.victory:
		#region
		if keyboard_check_pressed(vk_anykey) {
			if room_exists(room_next(room)) {	// if therer is a level after the current one, go to it. Else, go to menu 
				room_goto_next();
			} else {
				// setting the new highscore of steps used in the game, if was better
				if global.bestmovements != 0 {
					global.bestmovements = min(global.bestmovements, global.movements);
				} else {
					global.bestmovements = global.movements;	
				}
				// setting the new highscore of time spent in the game, if was better
				if global.besttime != 0 {
					global.besttime = min(global.besttime, global.time);
				} else {
					global.besttime = global.time;	
				}
				// svae the highscores
				ini_open("savedata.ini");
				ini_write_real("save1", "time", global.besttime);
				ini_write_real("save1", "movements", global.bestmovements);
				ini_close();
				room_goto(rMenu);	
			}
		} 
		break;
		#endregion;
	case game_state.defeat:
		#region
		if keyboard_check_pressed(vk_anykey) {
			global.level --;	// As we will add this variable every time the room is reseted, we need to substract 1 of it
			room_restart();		// restart the room
		}
		#endregion
}


