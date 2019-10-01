#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cmath>

#include "engine.h"

Engine::Engine()
{
}

void Engine::Render(GameState *g, int w, int h, uint32_t *pixels, int pitch)
{
	memset(pixels, 0x00, h * pitch);

	const int *map = g->GetMap();
	const uint32_t *texture = g->GetTextures();
	int mapw = g->GetMapWidth() - 1;

	// real position, direction, and camera plane vectors
	double posX = g->GetPosX();
	double posY = g->GetPosY();
	double dirX = g->GetDirX();
	double dirY = g->GetDirY();
	double planeX = g->GetPlaneX();
	double planeY = g->GetPlaneY();

	// ray direction + grid position vectors
	double cameraX, ray_dirX, ray_dirY;
	int mapX, mapY, stepX, stepY;

	// DDA variables
	double sideX, sideY, deltaX, deltaY, distance;

	// drawing variables
	int hit, color, line_height, line_start;
	double wallX;
	
	// for each x of the screen, cast ray
	for (int x = 0; x < w; x++) {

		// our current position on the grid
		mapX = int(posX);
		mapY = int(posY);

		// find direction of this ray
		cameraX = ((2 * x) / double(w)) - 1;
		ray_dirX = dirX + (planeX * cameraX);
		ray_dirY = dirY + (planeY * cameraX);

		deltaX = std::abs((double)1 / ray_dirX);
		if (ray_dirX < 0) {
			stepX = -1;
			sideX = (posX - mapX) * deltaX;
		} else {
			stepX = 1;
			sideX = (mapX + 1.0 - posX) * deltaX;
		}
		
		deltaY = std::abs((double)1 / ray_dirY);
		if (ray_dirY < 0) {
			stepY = -1;
			sideY = (posY - mapY) * deltaY;
		} else {
			stepY = 1;
			sideY = (mapY + 1.0 - posY) * deltaY;
		}

		// actually cast the ray, fast as possible!
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
			if (map[(mapY*mapw) + mapX] == 0) hit = 0;
		}

		int texX;
		if (hit > 0) {
			distance = (mapX - posX + (1 - stepX) / 2) / ray_dirX;
			wallX = posY + distance * ray_dirY;
			wallX -= floor(wallX);
			texX = int(wallX * 64.0);
			if (ray_dirX > 0) texX = 64 - texX - 1;
		} else {
			distance = (mapY - posY + (1 - stepY) / 2) / ray_dirY;
			wallX = posX + distance * ray_dirX;
			wallX -= floor(wallX);
			texX = int(wallX * 64.0);
			if (ray_dirY < 0) texX = 64 - texX - 1;
		}

		line_height = (int)(h / distance);
		line_start = (h / 2) + (line_height / 2);

		// don't draw outside the screen!
		if (line_start >= h) {
			line_height = line_height - (line_start - h - 1);
			line_start = h - 1;
		}
		if (line_start - line_height < 0) {
			line_height = line_start;
		}

		// draw a vertical strip of the screen
		for (int y = 0; y < line_height; y++) {
			int z = (line_start - (y+1)) * 256 - h * 128 + line_height * 128;
			int texY = ((z * 64) / line_height) / 256;
			color = texture[texX * 64 + texY];
			pixels[x + (w * (line_start - y))] = color;
		}
	}
}
