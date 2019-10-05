#ifndef MAP_H_
#define MAP_H_

#include <SDL2/SDL.h>
#include <cstdint>

class Map {
public:
	int *map;
	int width, height;
	SDL_Surface *texture;
	uint32_t textures[4096];
	Map();
	~Map();
};

#endif
