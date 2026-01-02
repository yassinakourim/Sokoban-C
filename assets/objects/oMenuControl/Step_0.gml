/// @description Actions

// chance menu item selected
var _change = keyboard_check_pressed(vk_down) - keyboard_check_pressed(vk_up);
var _previous_selected = selected;

// if there is chance...
if _change != 0 {
	selected = clamp(selected + _change, 0, ds_list_size(menu) - 1);
	if selected != _previous_selected {
		audio_play_sound(sndSelect, 100, false);	
	}
}

// when enter pressed, do what need to do based on the selected menu item
if keyboard_check_pressed(vk_enter) {
	switch(selected) {
		case 0:	// case of the first => Start game
			audio_play_sound(sndConfirmation, 100, false);
			room_goto(rLevel01);
			break;
		case 1:	// case of the second one => Credits
			room_goto(rCredits);
			audio_play_sound(sndConfirmation, 100, false);
			break;
		case 2:	// case of the thrid one =>  exit game
			game_end();
	}
}