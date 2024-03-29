#ifndef gamestate_h_
#define gamestate_h_

#include "map.h"
#include "util/vector.h"

#define MAP_WIDTH (10)
#define MAP_HEIGHT (10)

enum AppState {
	QUIT,
	RUNNING
};

class GameState {
	enum AppState appstate = RUNNING;

	// player state
	Vector position;
	Vector direction;
	Vector camera;
	double rot_speed = 3.0;
	double move_speed = 5.0;

	// map
	Map map;
public:
	GameState();

	// game state management
	int Running();
	void Quit();

/* player methods */
private:
	void Move(Vector *v, bool forward, double dt);
public:
	// player methods
	double GetPosX();
	double GetPosY();
	double GetDirX();
	double GetDirY();
	double GetPlaneX();
	double GetPlaneY();
	void Move(bool forward, double dt);
	void Strafe(bool left, double dt);
	void Rotate(bool left, double dt);
	
	// map methods
	int GetMapWidth();
	int GetMapHeight();
	const int *GetMap();
	const uint32_t *GetTextures();
};


#endif
