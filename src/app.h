#ifndef app_h_
#define app_h_

#include <SDL2/SDL.h>
#include "game/gamestate.h"
#include "display/display.h"

class App {
	GameState *gamestate;
	Display *display;


	// internal methods
	int Init();
	int Event(SDL_Event *Event);
	int Update(double dt);
	void Render();
	int Free();

public:
	App();
	~App();
	int Execute();
};

#endif
