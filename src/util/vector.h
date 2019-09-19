#ifndef vector_h_
#define vector_h_

class Vector {
protected:
	double x, y;
public:
	Vector(double x = 0, double y = 0);
	double GetX(), GetY();
	double SetX(double x), SetY(double y);
	void Rotate(double theta);
};


#endif
