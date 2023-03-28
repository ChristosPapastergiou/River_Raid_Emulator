#include <stdlib.h>

#include "state.h"
#include "raylib.h"
#include "interface.h"

Texture jet;
Texture warship;
Texture helicopter;

void interface_init(){
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

	jet = LoadTextureFromImage(LoadImage("assets/plane.png"));
	warship = LoadTextureFromImage(LoadImage("assets/ship.png"));
	helicopter = LoadTextureFromImage(LoadImage("assets/helicopter.png"));
}

void interface_close(){
	CloseWindow();
}

void interface_draw_frame(State state){
	BeginDrawing();

	ClearBackground(BLUE);

    StateInfo info = state_info(state);

    int x_offset = info->jet->rect.x - SCREEN_WIDTH/2;
    int y_offset = info->jet->rect.y - 650;

	DrawTexture(jet, info->jet->rect.x - x_offset, info->jet->rect.y - y_offset, BLACK);

	if(info->missile != NULL){
		DrawRectangle(info->missile->rect.x - x_offset + 15, info->missile->rect.y - y_offset, info->missile->rect.width, info->missile->rect.height, WHITE);
	}

	List obj_list = state_objects(state, info->jet->rect.y - y_offset, info->jet->rect.y - SCREEN_HEIGHT*2);

	for(ListNode node=list_first(obj_list); node!=LIST_EOF; node=list_next(obj_list, node)){
		Object obj = list_node_value(obj_list, node);

		if(obj->type == TERAIN){
			DrawRectangle(obj->rect.x - x_offset, obj->rect.y - y_offset, obj->rect.width, obj->rect.height, DARKGREEN);
		}else if(obj->type == HELICOPTER){
			DrawTexture(helicopter, obj->rect.x - x_offset, obj->rect.y - y_offset, YELLOW);
		}else if(obj->type == WARSHIP){
			DrawTexture(warship, obj->rect.x - x_offset, obj->rect.y - y_offset, DARKGRAY);
		}else if(obj->type == BRIDGE){
			DrawRectangle(obj->rect.x - x_offset, obj->rect.y - y_offset, obj->rect.width, obj->rect.height, RED);
		}
	}

	DrawText(TextFormat("%04i", info->score), 10, 10, 20, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	if(info->paused){
		DrawText(
			"PRESS [P] TO CONTINUE",
			GetScreenWidth() / 2 - MeasureText("PRESS [P] TO CONTINUE", 20) / 2,
		    GetScreenHeight() / 2 - 50, 20, BLACK
		);
		DrawText(
			"PRESS [ESC] TO EXIT",
			GetScreenWidth() / 2 - MeasureText("PRESS [ESC] TO EXIT", 20) / 2,
		    GetScreenHeight() / 2 - 30, 20, BLACK
		);
	}

	if(!info->playing){
		DrawText(
			"GAME OVER",
			GetScreenWidth() / 2 - MeasureText("GAME OVER", 20) / 2,
		    GetScreenHeight() / 2 - 100, 20, BLACK
		);
		DrawText(
			"___________",
			GetScreenWidth() / 2 - MeasureText("__________", 22) / 2,
		    GetScreenHeight() / 2 - 90, 20, BLACK
		);
		DrawText(
			"___________",
			GetScreenWidth() / 2 - MeasureText("__________", 22) / 2,
		    GetScreenHeight() / 2 - 120, 20, BLACK
		);
		DrawText(
			"PRESS [ENTER] TO PLAY",
			GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY", 20) / 2,
		    GetScreenHeight() / 2 - 50, 20, BLACK
		);
		DrawText(
			"PRESS [ESC] TO EXIT",
			GetScreenWidth() / 2 - MeasureText("PRESS [ESC] TO EXIT", 20) / 2,
		    GetScreenHeight() / 2 - 30, 20, BLACK
		);
	}

	EndDrawing();
}