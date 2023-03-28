#include <stdlib.h>

#include "state.h"
#include "ADTList.h"

struct state {
	List objects;
	struct state_info info;
};

// Making the object
static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

// Visible only to this file. Function for objects
static void add_objects(State state, float start_y) {
	for (int i = 0; i < BRIDGE_NUM; i++) {
		Object bridge = create_object(BRIDGE, 0, start_y - 4 * (i+1) * SPACING, SCREEN_WIDTH, 20);

		Object terain_left = create_object( TERAIN, 0, bridge->rect.y, rand() % (SCREEN_WIDTH/3), 4*SPACING);

		int width = (rand() % (SCREEN_WIDTH/2)) * 0.5;

		Object terain_right = create_object(TERAIN, SCREEN_WIDTH - width, bridge->rect.y, width, 4*SPACING);

		list_insert_next(state->objects, list_last(state->objects), terain_left);
		list_insert_next(state->objects, list_last(state->objects), terain_right);
		list_insert_next(state->objects, list_last(state->objects), bridge);

		// Adding 3 enemies before every bridge
		for (int j = 0; j < 3; j++) {
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0 ? create_object(WARSHIP, (SCREEN_WIDTH - 83)/2, y, 83, 30) : create_object(HELICOPTER, (SCREEN_WIDTH - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;

			list_insert_next(state->objects, list_last(state->objects), enemy);
		}
	}
}

State state_create() {
	State state = malloc(sizeof(*state));

	state->info.playing = true;
	state->info.paused = false;
	state->info.score = 0;
	state->info.missile = NULL;

	state->info.jet = create_object(JET, (SCREEN_WIDTH - 35)/2,  0, 35, 40);

	state->objects = list_create(NULL);
	add_objects(state, 0);

	return state;
}

StateInfo state_info(State state) {
	return &state->info;
}

List state_objects(State state, float y_from, float y_to) {
	List list = list_create(NULL);

	for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
		Object obj = list_node_value(state->objects, node);

		if(obj->rect.y <= y_from && obj->rect.y >= y_to){
			list_insert_next(list, LIST_BOF, obj);
		}
	}

	return list;
}

void state_update(State state, KeyState keys) {

	/**** Start-Pause-Exit ****/ 

	if(!state->info.playing && !keys->enter){
		return;
	}else if(!state->info.playing){
		state->info.playing = true;
		state_destroy(state);
		state_create();			
	}

	if(keys->esc) state_destroy(state);

	if(state->info.paused){
		if(keys->p){
			state->info.paused = true;	
		}else if(!keys->n){
			return;
		}
	}
	
	if(keys->p && !state->info.paused){
		state->info.paused = true;
	}else if(keys->p && state->info.paused){
		state->info.paused = false;
	}

	/**** Jet movement ****/

	if(keys->up){
		state->info.jet->rect.y -= 6;
	}else if(keys->down){
		state->info.jet->rect.y -= 2;
	}else{
		state->info.jet->rect.y -= 3;
	}

	if(keys->right){
		state->info.jet->rect.x += 3;
	}else if(keys->left){
		state->info.jet->rect.x -= 3;
	}

	/**** Helicopter & Warship movement ****/

	for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
		Object obj = list_node_value(state->objects, node);

		if(obj->type == HELICOPTER){
			obj->rect.x += obj->forward == true ?  4 : -4;
		}else if(obj->type == WARSHIP){
			obj->rect.x += obj->forward == true ?  3 : -3;
		}
	}

	/**** Enemy collisions ****/
	
	for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
		Object obj_1 = list_node_value(state->objects, node);

		if(obj_1->type == TERAIN || obj_1->type == HELICOPTER || obj_1->type == WARSHIP || obj_1->type == BRIDGE){
			if(CheckCollisionRecs(state->info.jet->rect, obj_1->rect)){
				state->info.playing = false;
				return;
			}
		}

		for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
			Object obj_2 = list_node_value(state->objects, node);
			
			if(obj_1->type == TERAIN || obj_1->type == HELICOPTER || obj_1->type == WARSHIP){
				if(obj_1->type == TERAIN){
					if(obj_2->type == HELICOPTER || obj_2->type == WARSHIP){
						if(CheckCollisionRecs(obj_1->rect, obj_2->rect)){
							obj_2->forward = obj_2->forward == true ? false : true; // Change direction of the enemy
						}	
					}
				}else if(obj_1->type == HELICOPTER || obj_1->type == WARSHIP){
					if(obj_2->type == TERAIN){
						if(CheckCollisionRecs(obj_1->rect, obj_2->rect)){
							obj_1->forward = obj_1->forward == true ? true : false; // Change direction of the enemy
						}	
					}
				}
			}
		}
	}

	// If there is no missile and space is active make one
	if(keys->space && state->info.missile == NULL){
		state->info.missile = create_object(MISSLE, state->info.jet->rect.x,  state->info.jet->rect.y, 4, 20);
	}

	/**** Missile movement & collisions ****/

	if(state->info.missile != NULL){

		state->info.missile->rect.y -= 10;

		// Destroy the missile at the end of the screen
		if (state->info.missile->rect.y < state->info.jet->rect.y - SCREEN_HEIGHT)  {
            state->info.missile = NULL;
            return;
        }

		// Making a previous node to be able to remove the correct object at the next step
		ListNode previous_node = LIST_BOF;

		for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
			Object obj = list_node_value(state->objects, node);

			if(obj->type == TERAIN){
				if(state->info.missile != NULL){
					if(CheckCollisionRecs(state->info.missile->rect, obj->rect)){
						state->info.missile = NULL;
					}
				}
			}else if(obj->type == HELICOPTER || obj->type == WARSHIP || obj->type == BRIDGE){
				if(state->info.missile != NULL){
					if(CheckCollisionRecs(state->info.missile->rect, obj->rect)){
						state->info.missile = NULL;
						list_remove_next(state->objects, previous_node);
						state->info.score += 10;
						return;
					}
				}
			}

			// Update the previous node
			previous_node = node;
		}
	}

	// Temp valriable for rect->y of the last bridge
	float last_bridge_y;

	// Finding the last bridge so we can add objects at the next step
	for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
		Object obj = list_node_value(state->objects, node);

		if(obj->type == BRIDGE) last_bridge_y = obj->rect.y;
	}

	if(abs(last_bridge_y - state->info.jet->rect.y) <= SCREEN_HEIGHT) add_objects(state, last_bridge_y); // Adding new objects
}

void state_destroy(State state){
	free(state);
}