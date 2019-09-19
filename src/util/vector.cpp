#include <cmath>
#include "vector.h"


Vector::Vector(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Vector::GetX()
{
	return x;
}

double Vector::GetY()
{
	return y;
}

double Vector::SetX(double x)
{
	return (this->x = x);
}

double Vector::SetY(double y)
{
	return (this->y = y);
}

void Vector::Rotate(double theta)
{
	double temp = x;
	x = ((temp * cos(theta)) - (y * sin(theta)));
	y = ((temp * sin(theta)) + (y * cos(theta)));
}
