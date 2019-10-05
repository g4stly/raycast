#include <cstdint>
#include <SDL2/SDL.h>
#include "display.h"

#define WINDOW_WIDTH (860)
#define WINDOW_HEIGHT (640)

int Display::Init()
{
	// initialize SDL
	int error = SDL_Init(SDL_INIT_EVERYTHING);
	if (error) {
		SDL_Log("Failed to initialize %s\n", SDL_GetError());
		return error;
	}

	// create window
	window = SDL_CreateWindow(
		"Raycast Experiment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (!window) {
		SDL_Log("Failed to create window: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	// make sure our pixel format is supported
	SDL_RendererInfo info;
	error = SDL_GetRendererInfo(renderer, &info);
	if (error < 0) {
		SDL_Log("Failed to get renderer info: %s\n",
			SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return error;
	}

	int supported = 0;
	for (int i = info.num_texture_formats; i > 0; i--) {
		supported = info.texture_formats[i-1]
			== SDL_PIXELFORMAT_RGBA32;
		if (supported) break;
	}
	if (!supported) {
		SDL_Log("RGBA32 pixel format unsupported\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	// create background texture
	background = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!background) {
		SDL_Log("Failed to create background: %s\n",
			SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	return 0;
}

int Display::KeyDown(SDL_Keycode key, GameState *g, double dt)
{
	switch (key) {
	case SDLK_q:
		g->Quit();
		return 0;
	case SDLK_w:
		g->Move(true, dt);
		break;
	case SDLK_s:
		g->Move(false, dt);
		break;
	case SDLK_a:
		g->Strafe(true, dt);
		break;
	case SDLK_d:
		g->Strafe(false, dt);
		break;
	case SDLK_k:
		g->Rotate(true, dt);
		break;
	case SDLK_l:
		g->Rotate(false, dt);
		break;
	}

	return 1;
}

int Display::Event(SDL_Event *ev)
{
	switch (ev->type) {
	case SDL_KEYDOWN:
		keypress_map[ev->key.keysym.sym] = 1;
		break;
	case SDL_KEYUP:
		keypress_map.erase(ev->key.keysym.sym);
		break;
	}
	return 1;
}

int Display::Update(GameState *g, double dt)
{
	int rv = 1;
	for (auto key : keypress_map) {
		rv = rv && KeyDown(key.first, g, dt);
	}

	return rv;
}

void Display::Render(GameState *g)
{
	int pitch;
	void *pixels;

	if (SDL_LockTexture(background, NULL, &pixels, &pitch) < 0) {
		SDL_Log("Failure locking texture: %s\n", SDL_GetError());
		g->Quit();	// recovery?
		return;
	}

	engine.Render(g, WINDOW_WIDTH, WINDOW_HEIGHT,
		(uint32_t*)pixels, pitch);
	SDL_UnlockTexture(background);

	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderPresent(renderer);
}

int Display::Free()
{
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
