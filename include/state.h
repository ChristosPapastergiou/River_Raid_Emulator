#pragma once

#include "raylib.h"
#include "ADTList.h"

#define SPACING 200
#define BRIDGE_NUM 20
#define SCREEN_WIDTH 450
#define SCREEN_HEIGHT 800

typedef enum{
	TERAIN, HELICOPTER, WARSHIP, JET, MISSLE, BRIDGE
}ObjectType;

typedef struct object{
	ObjectType type;	// Type (Ground / Helicopter / Ship / Aircraft / Rocket / Bridge)
	Rectangle rect;		// Position and size of the object
	bool forward;		// True: the object moves to the right
}* Object;

// General information about the status of the game
typedef struct state_info{
	Object jet;
	Object missile;
	bool playing;
	bool paused;
	int score;
}* StateInfo;

// True if key is pressed
typedef struct key_state{
	bool up;						
	bool down;
	bool left;
	bool right;
	bool space;
	bool enter;
	bool n;
	bool p;
	bool esc;
}* KeyState;

typedef struct state* State;

// Creates and returns the original state of the game
State state_create();

// Returns the basic game information to state mode
StateInfo state_info(State state);

// Returns a list of all objects in the game in the state state, whose coordinate y is between the y_from and y_to.
List state_objects(State state, float y_from, float y_to);

// Updates the state status of the game after 1 frame has elapsed. 
// The keys contain the keys that were pressed during this frame.
void state_update(State state, KeyState keys);

// Destroying the state
void state_destroy(State state);