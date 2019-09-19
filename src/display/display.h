#ifndef display_h_
#define display_h_

#include <unordered_map>
#include <SDL2/SDL.h>
#include "game/gamestate.h"
#include "util/vector.h"
#include "engine.h"

class Display {
	// sdl state
	SDL_Window *window;
	Vector screensz;

	SDL_Renderer *renderer;
	SDL_Texture *background;

	// display state
	std::unordered_map<SDL_Keycode, int> keypress_map;
	Engine engine;

	// methods
	int KeyDown(SDL_Keycode, GameState *g, double dt);
public:
	int Init();
	int Event(SDL_Event *ev);
	int Update(GameState *g, double dt);
	void Render(GameState *g);
	int Free();
};

#endif
