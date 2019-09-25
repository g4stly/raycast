#ifndef MAP_H_
#define MAP_H_

#include <cstdint>

class Map {
public:
	int *map;
	int width, height;
	uint32_t textures[4096];
	Map();
};

#endif
