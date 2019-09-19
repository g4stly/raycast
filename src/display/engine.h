#ifndef engine_h_
#define engine_h_

#include <cstdint>
#include "game/gamestate.h"
#include "util/vector.h"

class Engine {
public:
	Engine();
	void Render(GameState *g, int w, int h, uint32_t *pixels, int pitch);
};

#endif
