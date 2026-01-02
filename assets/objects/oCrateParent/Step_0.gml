/// @description Movement

// if place snapped, stop movement - used after the player start to move one crate
if place_snapped(64, 64) {
	speed = 0;	
}