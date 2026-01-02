/// @description Draw menu items

// draw title
text("The Storage Manager", room_width / 2, 20, fa_center, fa_top, Font60, 10, c_aqua);

// draw menu
var _number = ds_list_size(menu);

for (var i = 0; i < _number; i ++) {
	var _text = string(menu[| i]);
	var _color = c_yellow;
	if selected == i {
		_color = c_lime;	
	}
	text(_text, room_width / 2, menu_start_position + i * menu_separation, fa_center, fa_top, FontTitle, 5, _color);	
}


// draw menu arrow
angle += 3;
var _dx = lengthdir_x(delta_x, angle);
var _delta_y = 25;
var _shadow_offset = 5;
var _color = c_lime;
// left arrow
draw_sprite_ext(sArrow, 0, room_width / 2 - 180 + _shadow_offset + _dx, menu_start_position + selected * menu_separation + _delta_y + _shadow_offset, 1, 1, 0, c_black, 0.5);
draw_sprite_ext(sArrow, 0, room_width / 2 - 180 + _dx, menu_start_position + selected * menu_separation + _delta_y, 1, 1, 0, _color, 1);
draw_set_color(c_white);
// right arrow
draw_sprite_ext(sArrow, 0, room_width / 2 + 180 + _shadow_offset - _dx, menu_start_position + selected * menu_separation + _delta_y + _shadow_offset, -1, 1, 0, c_black, 0.5);
draw_sprite_ext(sArrow, 0, room_width / 2 + 180 - _dx, menu_start_position + selected * menu_separation + _delta_y, -1, 1, 0, _color, 1);
draw_set_color(c_white);

// draw highscores
// best time
if global.besttime = 0 {
	text("No best time yet!", 20, room_height - 20, fa_left, fa_bottom, FontScore, 2, c_white);
} else {
	text("Best time: " + string(global.besttime), 20, room_height - 20, fa_left, fa_bottom, FontScore, 2, c_white);
}
// best movement
if global.bestmovements = 0 {
	text("No best number of steps yet!", room_width - 20, room_height - 20, fa_right, fa_bottom, FontScore, 2, c_white);
} else {
	text("Best number of steps: " + string(global.bestmovements), room_width - 20, room_height - 20, fa_right, fa_bottom, FontScore, 2, c_white);
}