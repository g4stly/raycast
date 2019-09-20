#include <cstdio>
#include <cmath>
#include "gamestate.h"

static const double PI = 4.0 * atan(1.0);

GameState::GameState()
{
	position.SetX(5);
	position.SetY(5);

	/*\
	 * FOV is derived from a ratio between the 
	 * direction and camera vectors 
	\*/
	direction.SetX(1);
	direction.SetY(0);

	camera.SetX(0);
	camera.SetY(-.66);
}

int GameState::Running()
{
	return appstate != QUIT;
}

void GameState::Quit()
{
	appstate = QUIT;
}

double GameState::GetPosX()
{
	return position.GetX();
}

double GameState::GetPosY()
{
	return position.GetY();
}

void GameState::Move(Vector *v, bool forward, double dt)
{
	double delta = forward
		? v->GetX() * (move_speed * dt)
		: 0 - v->GetX() * (move_speed * dt);
	position.SetX(position.GetX() + delta);

	delta = forward
		? v->GetY() * (move_speed * dt)
		: 0 - v->GetY() * (move_speed * dt);
	position.SetY(position.GetY() + delta);
}

void GameState::Move(bool forward, double dt)
{
	Move(&direction, forward, dt);
}

void GameState::Strafe(bool left, double dt)
{
	Vector temp(direction);
	temp.Rotate(0.5 * PI);
	Move(&temp, left, dt);
}

double GameState::GetDirX()
{
	return direction.GetX();
}

double GameState::GetDirY()
{
	return direction.GetY();
}

double GameState::GetPlaneX()
{
	return camera.GetX();
}

double GameState::GetPlaneY()
{
	return camera.GetY();
}

void GameState::Rotate(bool left, double dt)
{
	double theta = dt * (left ? rot_speed : -rot_speed);
	direction.Rotate(theta);
	camera.Rotate(theta);
}
