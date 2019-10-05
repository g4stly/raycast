#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cmath>

#include "engine.h"

#define VERTICAL (-1)
#define HORIZONTAL (1)
#define TEX_SZ (64)

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

	// wall drawing variables
	int hit, color, line_height, line_start, shrink;
	int draw_height, draw_start, texX, texY, z;
	double wallX;

	// floor drawing variables
	double floorX, floorY, cur_floorX, cur_floorY;
	double weight, currentDist;
	
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
				hit = HORIZONTAL;
			} else {
				sideY += deltaY;
				mapY += stepY;
				hit = VERTICAL;
			}
			if (map[(mapY*mapw) + mapX] == 0) hit = 0;
		}

		if (hit == HORIZONTAL) {
			distance = (mapX - posX + (1 - stepX) / 2) / ray_dirX;
			wallX = posY + distance * ray_dirY;
			wallX -= floor(wallX);
			texX = int(wallX * TEX_SZ);
			if (ray_dirX > 0) texX = TEX_SZ - texX - 1;
		} else {
			distance = (mapY - posY + (1 - stepY) / 2) / ray_dirY;
			wallX = posX + distance * ray_dirX;
			wallX -= floor(wallX);
			texX = int(wallX * TEX_SZ);
			if (ray_dirY < 0) texX = TEX_SZ - texX - 1;
		}

		line_height = (int)(h / distance);
		line_start = (h / 2) + (line_height / 2);
		draw_height = line_height;
		draw_start = line_start;

		// don't draw outside the screen!
		// the draw_ variables enable us to skip
		// iterating over off-screen pixels but still
		// draw the true texture color as if the line were 
		// being drawn off screen
		if (draw_start >= h) {
			draw_height = draw_height - (draw_start - h - 1);
			draw_start = h - 1;
		}
		if (draw_start - draw_height < 0) {
			draw_height = draw_start;
		}

		// hacky bullshit sorry!
		// draw a vertical strip of the screen from bottom up
		for (int y = 0; y < draw_height; y++) {
			shrink = line_start - draw_start;
			z = (line_start - (y+1)) * 256 - h * 128 + (line_height - shrink) * 128;
			texY = ((z * TEX_SZ) / (line_height + shrink)) / 256;
			color = texture[texX * TEX_SZ + texY];
			pixels[x + (w * (draw_start - y))] = color;
		}



		// position of texel at bottom of wall
		if (hit == HORIZONTAL) {
			floorX = ray_dirX > 0 ? mapX : mapX + 1.0;
			floorY = mapY + wallX;
		} else {
			floorX = mapX + wallX;
			floorY = ray_dirY > 0 ? mapY : mapY + 1.0;
		}

		// from bottom of wall line to bottom of screen
		for (int y = draw_start; y < h; y++) {
			currentDist = h / ((2.0 * y) - h);
			weight = (currentDist - 0.0) / (distance - 0.0);
			cur_floorX = weight * floorX + (1.0 - weight) * posX;
			cur_floorY = weight * floorY + (1.0 - weight) * posY;
			texX = int(cur_floorX * TEX_SZ) % TEX_SZ;
			texY = int(cur_floorY * TEX_SZ) % TEX_SZ;

			color = texture[texX * TEX_SZ + texY];
			pixels[x + (w *  y)] = color;
			// symmetric point for ceiling
			pixels[x + (w * (h - y))] = 0xFF0000;
		}

	}
}
