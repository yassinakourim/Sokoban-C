//Player movement
function move(_direction, _speed) {
	var _dx = lengthdir_x(global.grid_size[0], _direction);
	var _dy = lengthdir_y(global.grid_size[1], _direction);
	if place_free(x + _dx, y + _dy) {
		motion_set(_direction, _speed);
		global.movements ++;
	} else {
		var _instance = instance_place(x + _dx, y + _dy, oCrateParent);
		if instance_exists(_instance) {
			if place_free(x + 2 * _dx, y + 2 * _dy) {
				motion_set(_direction, _speed);
				global.movements ++;
				with(_instance) {
					motion_set(_direction, _speed);	
				}
			}
		}
	}
}

// draw text with shadow
function text(_text, _x, _y, _halign, _valign, _font, _shadow_offset, _color) {
	draw_set_font(_font);
	draw_set_halign(_halign);
	draw_set_valign(_valign);
	draw_set_color(c_black);
	draw_set_alpha(0.5);
	draw_text(_x + _shadow_offset, _y + _shadow_offset, string(_text));
	draw_set_color(_color);
	draw_set_alpha(1);
	draw_text(_x, _y, string(_text));
	draw_set_color(c_white)
}