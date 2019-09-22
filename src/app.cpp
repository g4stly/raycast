#include <cstdint>
#include <app.h>

App::App()
{
	display = new Display();
	gamestate = new GameState();
}

App::~App()
{
	delete(gamestate);
	delete(display);
}

int App::Execute()
{
	int error = Init();
	if (error) return error;

	double dt;
	SDL_Event ev;
	uint64_t prev, cur = SDL_GetPerformanceCounter();
	while (gamestate->Running()) {
		prev = cur;
		cur = SDL_GetPerformanceCounter();
		dt = (double)((cur - prev) * 1000)
			/ SDL_GetPerformanceFrequency();
		dt = dt * 0.001;

		while (SDL_PollEvent(&ev) && (prev = Event(&ev)));
		if (prev && Update(dt)) Render();
	}
	return Free();
}

int App::Init()
{
	return display->Init();
}

int App::Event(SDL_Event *event)
{	// use gamestate to set gamestate?!
	if (event->type == SDL_QUIT) {
		gamestate->Quit();
		return 0;
	}
	return display->Event(event);
}

int App::Update(double dt)
{
	return display->Update(gamestate, dt);
}

void App::Render()
{
	display->Render(gamestate);
}

int App::Free()
{
	return display->Free();
}

int main(int argc, char **argv)
{
	App a;
	return a.Execute();
}
