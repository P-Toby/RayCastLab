#include "Shapes.h"

PPlane::PPlane(Vec normal, float _d, Color color)
{
	//Constructor
	n = normal;
	d = _d;
	c = color;
}

void PPlane::test(Ray& ray, HitData& hit) 
{
	///A(x - x1) + B(y - y1) + C(z - z1) = 0
	///0(x - 0) + 	0(y - 0	) + (-1)(z - 1000) = 0
	//Plane eq: - z + 1000 = 0
	
	
}

Vec PPlane::normal(Vec &point)
{
	//For a plane the normal is the same regardless of intersection point
	return n;
}

Color PPlane::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	//For now we just return the color, no shading
	return c;
}
