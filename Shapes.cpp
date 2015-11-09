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
	//Calculate the t value
	float temp = (-d - (n.Dot(ray.o)));
	float tempDivisor = n.Dot(ray.d);
	float t;

	//Make sure we do not divide by 0
	if (tempDivisor != 0)
	{
		t = temp / tempDivisor; //Calculate the t value

		if (hit.t == -1 && t > 0) //Initial value stored in hit.t if t is larger than 0
		{
			hit.t = t;
			hit.lastShape = this;
		}
		else if (t < hit.t && t > 0) //t stored if smaller than hit.t and greater than zero.
		{
			hit.t = t;
			hit.lastShape = this;
		}
	}
	//If neither if statment runs then there was no hit and the resulting color is black.
	
}

Vec PPlane::normal(Vec &point)
{
	//For a plane the normal is the same regardless of intersection point
	return n;
}

//Used by all classes
Color Shape::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	//For now we just return the color, no shading
	return c;
}
