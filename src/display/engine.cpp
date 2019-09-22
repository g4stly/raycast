#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cmath>

#include "engine.h"

const static int mock_map[20*20] = {
	1, 8, 1, 8, 8, 8, 1, 8, 1, 1, 8, 1, 8, 8, 8, 1, 8, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 4,
	4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
	6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 4,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
	6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 4, 4, 1, 4, 1, 4, 1, 1, 1, 4, 4, 1, 4, 1, 4, 1, 1
};

struct RayCastArgs {
	// arguments 
	const int *map;
	int mapw, maph;
	int screenw, screenh;
	double posX, posY;
	double dirX, dirY;
	double planeX, planeY;
	uint32_t *pixels;

	// ray direction + grid position vectors
	double cameraX, ray_dirX, ray_dirY;
	int mapX, mapY, stepX, stepY;
	// DDA variables
	double sideX, sideY, deltaX, deltaY, distance;
	// drawing variables
	int hit, color, line_height, line_start;
};

Engine::Engine()
{
}

static void rayCaster(RayCastArgs *args)
{
	RayCastArgs a = *args;	// use reference?
	for (int x = 0; x < a.screenw; x++) {
		a.cameraX = ((2 * x) / double(a.screenw)) - 1;
		a.ray_dirX = a.dirX + (a.planeX * a.cameraX);
		a.ray_dirY = a.dirY + (a.planeY * a.cameraX);

		a.mapX = int(a.posX);
		a.mapY = int(a.posY);

		a.deltaX = std::abs((double)1 / a.ray_dirX);
		a.deltaY = std::abs((double)1 / a.ray_dirY);

		if (a.ray_dirX < 0) {
			a.stepX = -1;
			a.sideX = (a.posX - a.mapX) * a.deltaX;
		} else {
			a.stepX = 1;
			a.sideX = (a.mapX + 1.0 - a.posX) * a.deltaX;
		}
		
		if (a.ray_dirY < 0) {
			a.stepY = -1;
			a.sideY = (a.posY - a.mapY) * a.deltaY;
		} else {
			a.stepY = 1;
			a.sideY = (a.mapY + 1.0 - a.posY) * a.deltaY;
		}

		// -1 is y 1 is x
		a.hit = 0;
		while (!a.hit) {
			if (a.sideX < a.sideY) {
				a.sideX += a.deltaX;
				a.mapX += a.stepX;
				a.hit = 1;
			} else {
				a.sideY += a.deltaY;
				a.mapY += a.stepY;
				a.hit = -1;
			}
			if (a.map[(a.mapY*a.mapw) + a.mapX] == 0) a.hit = 0;
		}

		if (a.hit > 0) {
			a.distance = (a.mapX - a.posX + (1 - a.stepX) / 2)
				/ a.ray_dirX;
		} else {
			a.distance = (a.mapY - a.posY + (1 - a.stepY) / 2)
				/ a.ray_dirY;
		}

		a.line_height = (int)(a.screenh / a.distance);
		a.line_start = (a.screenh / 2) + (a.line_height / 2);
		// a.line_height = a.line_height * .4;

		if (a.line_start >= a.screenh) {
			a.line_height = a.line_height - (a.line_start - a.screenh - 1);
			a.line_start = a.screenh - 1;
		}
		if (a.line_start - a.line_height < 0) {
			a.line_height = a.line_start;
		}

		for (int y = a.line_height; y >= 0; y--) {
			a.color = a.hit > 0 ? 0xFFFFFFFF : 0xFFD0D0D0;
			a.pixels[x + (a.screenw * (a.line_start - y))] = a.color;
		}
	}
}

void Engine::Render(GameState *g, int w, int h, uint32_t *pixels, int pitch)
{
	memset(pixels, 0x00, h * pitch);

	// real position, direction, and camera plane vectors
	RayCastArgs args = {
		mock_map, 19, 19, w, h,
		g->GetPosX(), g->GetPosY(),
		g->GetDirX(), g->GetDirY(),
		g->GetPlaneX(), g->GetPlaneY(),
		pixels
	};

	rayCaster(&args);
}
