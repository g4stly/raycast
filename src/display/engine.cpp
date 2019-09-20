#include <cstring>
#include <iostream>
#include <cstdint>
#include <cmath>

#include "engine.h"

#define posX (0)
#define posY (1)
#define dirX (2)
#define dirY (3)
#define planeX (4)
#define planeY (5)

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

const static int mock_map2[20*20] = {
	1, 8, 1, 8, 8, 8, 1, 8, 1, 1, 8, 1, 8, 8, 8, 1, 8, 1, 1,
	1, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 2, 0, 0, 2, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 4,
	4, 0, 0, 0, 2, 0, 2, 2, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 4,
	6, 0, 0, 0, 0, 0, 0, 0, 2, 6, 2, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1,
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

Engine::Engine()
{
}

static void rayCaster(const int *map,
	int mapw, const double *vars,
	int skew, int screenw, int screenh,
	uint32_t *pixels)
{
	// ray direction vector 
	double cameraX, ray_dirX, ray_dirY;

	// grid position vector
	int mapX, mapY;

	// added to grid vector on wall miss
	int stepX, stepY;

	// DDA variables
	double sideX, sideY, deltaX, deltaY, target_dist;

	// drawing variables
	int hit, color, line_height, line_start;

	for (int x = 0; x < screenw; x++) {
		cameraX = ((2 * x) / double(screenw)) - 1;
		ray_dirX = vars[dirX] + (vars[planeX] * cameraX);
		ray_dirY = vars[dirY] + (vars[planeY] * cameraX);

		mapX = int(vars[posX]);
		mapY = int(vars[posY]);

		deltaX = std::abs((double)1 / ray_dirX);
		deltaY = std::abs((double)1 / ray_dirY);

		if (ray_dirX < 0) {
			stepX = -1;
			sideX = (vars[posX] - mapX) * deltaX;
		} else {
			stepX = 1;
			sideX = (mapX + 1.0 - vars[posX]) * deltaX;
		}
		
		if (ray_dirY < 0) {
			stepY = -1;
			sideY = (vars[posY] - mapY) * deltaY;
		} else {
			stepY = 1;
			sideY = (mapY + 1.0 - vars[posY]) * deltaY;
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
			if (map[(mapY*mapw) + mapX] == 0) hit = 0;
		}

		if (hit > 0) {
			target_dist = (mapX - vars[posX] + (1 - stepX) / 2) / ray_dirX;
		} else {
			target_dist = (mapY - vars[posY] + (1 - stepY) / 2) / ray_dirY;
		}

		line_height = (int)(screenh / target_dist);
		line_start = (-line_height / 2) + ((screenh / 2) - skew * line_height);
		if (line_start < 0) line_start = 0;
		if (line_height > screenh) line_height = screenh;

		for (int y = 0; y < line_height; y++) {
			color = hit > 0 ? 0xFFFFFFFF : 0xFFD0D0D0;
			pixels[x + (screenw * (line_start + y))] = color;
		}
	}
}

void Engine::Render(GameState *g, int w, int h, uint32_t *pixels, int pitch)
{
	memset(pixels, 0x00, h * pitch);

	// real position, direction, and camera plane vectors
	const double vars[6] = {
		g->GetPosX(),
		g->GetPosY(),
		g->GetDirX(),
		g->GetDirY(),
		g->GetPlaneX(),
		g->GetPlaneY()
	};

	rayCaster(mock_map, 19, vars, 0, w, h, pixels);
}

#undef posX
#undef posY
#undef dirX
#undef dirY
#undef planeX
#undef planeY
