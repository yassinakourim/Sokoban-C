/// @description Draw GUI

switch(state) {
	case game_state.starting:	// draw starting message
		text("Put all crates at their places", room_width / 2, room_height / 2, fa_center, fa_middle, FontTitle, 5, c_lime);
		break;
	case game_state.running:	// draw game GUI
		text("Missplaced crates: " + string(number_of_missing_crates), 20, 20, fa_left, fa_top, FontScore, 2, c_white);	// draw number of missplaced crates
		text("Storage facility number: " + string(global.level), room_width - 20, 20, fa_right, fa_top, FontScore, 2, c_white);	// draw level number
		text("Press ESC to restart level", 20, room_height - 20, fa_left, fa_bottom, FontScore, 2, c_white);	// draw message on how to restart level
		text("Number of Steps :" + string(global.movements) + "\nTime:" + string(floor(global.time)), room_width - 20, room_height - 20, fa_right, fa_bottom, FontScore, 2, c_white);	// draw scores
		break;
	case game_state.victory:
		if room_exists(room_next(room)) {
				text("Good job!\nNow go to another storage!\n Hurry!!!", room_width / 2, room_height / 2, fa_center, fa_middle, FontTitle, 5, c_aqua);	// draw level completion message
			} else {
				// text score local variables
				var _time_text;
				var _movement_text;
				// time text
				if global.besttime = 0 {
					_time_text = "Time spent: " + string(global.time);
				} else {
					if global.time < global.besttime {
						_time_text = "Time spent: " + string(global.time) + " <= New best!!!";	
					} else {
						_time_text = "Time spent: " +string(global.time) + " Best so far: " + string(global.besttime);	
					}
				}
				// movement text
				if global.bestmovements = 0 {
					_movement_text = "Movements used: " + string(global.movements);
				} else {
					if global.movements < global.bestmovements {
						_movement_text = "Movements used: " + string(global.movements) + " <= New best!!!";	
					} else {
						_movement_text = "Movements used: " +string(global.movements) + " Best so far: " + string(global.bestmovements);	
					}
				}
				// text
				var _text = _time_text + "\n" + _movement_text;
				// draw_text
				text(_text, room_width / 2, room_height / 2, fa_center, fa_middle, FontTitle, 5, c_aqua);
			}
		break;
	case game_state.defeat:
		text("Messed up? Don't do that again!", room_width / 2, room_height / 2, fa_center, fa_middle, FontTitle, 5, c_red);	// draw restart message
		break;
}