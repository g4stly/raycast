#include <cstring>
#include <iostream>
#include <cstdint>
#include <cmath>

#include "engine.h"

const static int map[20][20] = {
	{1, 8, 1, 8, 8, 8, 1, 8, 1, 1, 8, 1, 8, 8, 8, 1, 8, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 4},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
	{3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
	{6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 4},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
	{3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
	{6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 4, 4, 1, 4, 1, 4, 1, 1, 1, 4, 4, 1, 4, 1, 4, 1, 1}
};

Engine::Engine()
{
}

// this engine was largely inspired by the raycasting tutorial at
// https://lodev.org/cgtutor/raycasting.html
void Engine::Render(GameState *g, int w, int h, uint32_t *pixels, int pitch)
{
	memset(pixels, 0x00, h * pitch);

	// real position, direction, and camera plane vectors
	double posX = g->GetPosX();
	double posY = g->GetPosY();
	double dirX = g->GetDirX();
	double dirY = g->GetDirY();
	double planeX = g->GetPlaneX();
	double planeY = g->GetPlaneY(); 

	// ray direction vector 
	double cameraX, ray_dirX, ray_dirY;

	// grid position vector
	int mapX, mapY;

	// added to grid vector on wall miss
	int stepX, stepY;

	// DDA variables
	double sideX, sideY, deltaX, deltaY, target_dist;

	// drawing variables
	int hit, color, line_height, line_start, line_end;

	for (int x = 0; x < w; x++) {
		cameraX = ((2 * x) / double(w)) - 1;
		ray_dirX = dirX + (planeX * cameraX);
		ray_dirY = dirY + (planeY * cameraX);

		mapX = int(posX);
		mapY = int(posY);

		deltaX = std::abs((double)1 / ray_dirX);
		deltaY = std::abs((double)1 / ray_dirY);

		if (ray_dirX < 0) {
			stepX = -1;
			sideX = (posX - mapX) * deltaX;
		} else {
			stepX = 1;
			sideX = (mapX + 1.0 - posX) * deltaX;
		}
		
		if (ray_dirY < 0) {
			stepY = -1;
			sideY = (posY - mapY) * deltaY;
		} else {
			stepY = 1;
			sideY = (mapY + 1.0 - posY) * deltaY;
		}

		// -1 is y 1 is x
		hit = 0;
		while (!hit) {
			if (sideX < sideY) {
				sideX += deltaX;
				mapX += stepX;
				hit = 1;
			} else {
				sideY += deltaY;
				mapY += stepY;
				hit = -1;
			}
			if (map[mapX][mapY] == 0) hit = 0;
		}

		if (hit > 0) {
			target_dist = (mapX - posX + (1 - stepX) / 2) / ray_dirX;
		} else {
			target_dist = (mapY - posY + (1 - stepY) / 2) / ray_dirY;
		}

		line_height = (int)(h / target_dist);
		line_start = (-line_height / 2) + (h / 2);
		if (line_start < 0) line_start = 0;
		if (line_height > h) line_height = h;

		for (int y = 0; y < line_height; y++) {
			color = hit > 0 ? 0xFFFFFFFF : 0xFFD0D0D0;
			pixels[x + (w * (line_start + y))] = color;
		}
	}
}
