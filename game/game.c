#include <stdio.h>
#include <stdlib.h>

#include "state.h"
#include "raylib.h"
#include "interface.h"

State state;

void update_and_draw(){
	struct key_state keys = {
		.up = IsKeyDown(KEY_UP),
		.down = IsKeyDown(KEY_DOWN),
		.left = IsKeyDown(KEY_LEFT),
		.right = IsKeyDown(KEY_RIGHT),
		.enter = IsKeyPressed(KEY_ENTER),
		.space = IsKeyPressed(KEY_SPACE),
		.n = IsKeyPressed(KEY_N),
		.p = IsKeyPressed(KEY_P),
		.esc = IsKeyPressed(KEY_ESCAPE),
	};
	state_update(state, &keys);
	interface_draw_frame(state);
}

int main(){
	state = state_create();
	interface_init();
	start_main_loop(update_and_draw);
	interface_close();
	state_destroy(state);

	return 0;
}