#include "Shapes.h"

//---------------------------------------------------------------------
//PPlane functions
//---------------------------------------------------------------------
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

Color PPlane::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	return c;
}
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//PSphere functions
//---------------------------------------------------------------------
PSphere::PSphere(Vec _center, float _radius, Color _color)
{
	center = _center;
	radius = _radius;
	radius2 = _radius;
	c = _color;
}

void PSphere::test(Ray& ray, HitData& hit)
{
	float min_t;
	float t[2];
	float b = ray.d.Dot(ray.o - center);
	float w = ((ray.o - center).Dot(ray.o - center)) - pow(radius, 2);
	t[0] = -b + sqrt(pow(b, 2) - w);
	t[1] = -b - sqrt(pow(b, 2) - w);
	
	//Find which t is the smallest value
	if (t[0] < t[1])
	{
		min_t = t[0];
	}
	else if (t[0] > t[1])
	{
		min_t = t[1];
	}

	//Determine if hit
	if (hit.t == -1 && min_t > 0)
	{
		hit.t = min_t;

		if (pow(b, 2) - w > 0) //Only store shape if b^2 - w is greater than zero
		{
			hit.lastShape = this;
		}
	}
	else if (min_t < hit.t && min_t > 0)
	{
		hit.t = min_t;

		if (pow(b, 2) - w > 0)
		{
			hit.lastShape = this;
		}
	}
}
//vvvvvvv NOT SURE IF WORKING JUST YET (Need to implement lighting) vvvvvvv
Vec PSphere::normal(Vec& point)
{
	//Find vector from center to point on surface and normalize it
	Vec a  = point - center;
	a.Normalize();

	return a;
}

Color PSphere::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	return c;
}
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//PTriangle functions
//---------------------------------------------------------------------
PTriangle::PTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color)
{

}

void PTriangle::test(Ray& ray, HitData& hit)
{

}

Vec PTriangle::normal(Vec& point)
{
	///TODO: Calculate normal
	return nor;
}

Color PTriangle::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	return c;
}
//---------------------------------------------------------------------