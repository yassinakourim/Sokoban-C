/// @description Actions

// if the player is snapeed to the grid
if place_snapped(global.grid_size[0], global.grid_size[1]) {
	speed = 0;
	image_speed = 0;
	image_index = 0;
	// if the player is active...
	// the "move()" function is set at "Functions" script inside "Scripts" folder
	if active {
		switch(keyboard_key) {
			case vk_up:
				sprite_index = sPlayerFacingUp;	// set sprite of the player loking up
				move(90, spd);	// aplly movement
				break;
			case vk_down:
				sprite_index = sPlayerFacingDown;	// set the sprite of the player looking down
				move(270, spd);	// aplly movement
				break;
			case vk_right:
				sprite_index = sPlayerFacingRight;	// set the sprite of the player looking right
				move(0, spd);	// aplly movement
				break;
			case vk_left:
				sprite_index = sPlayerFacingLeft;	// set the sprite of the player looking left
				move(180, spd);	// aplly movement
				break;
		}
	}
} else {
	// if the player is not snapped, means that he is moveing
	image_speed = 1;	// start animation
	// footsteps sound
	if !audio_is_playing(sndFootStep1) and !audio_is_playing(sndFootStep2) {
		var _audio = choose(sndFootStep1, sndFootStep2);
		audio_play_sound(_audio, 100, false);
	}
}