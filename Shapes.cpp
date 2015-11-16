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

		if (hit.t == -1 && t > 0 || t < hit.t && t > 0) //Initial value stored in hit.t if t is larger than 0
		{
			hit.t = t;
			hit.lastShape = this;
			hit.color = c;
			hit.lastNormal = normal(ray.o + ray.d*t); //Save normal
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
	Vec intersectionPoint = r.o + r.d*h.t;
	Vec n = normal(intersectionPoint);
	Vec lightDir = (light - intersectionPoint);
	lightDir.Normalize();
	float angle = n.Dot(lightDir);

	Color dLight = { 255, 255, 255 };
	Color ambLight = { 50, 50, 50 };

	float rgb[3];

	rgb[0] = ((dLight.r / 255.0f) * (h.lastShape->c.r / 255.0f) * angle + (ambLight.r / 255.0f) * (h.lastShape->c.r / 255.0f)) * 255.0f; //red
	rgb[1] = ((dLight.g / 255.0f) * (h.lastShape->c.g / 255.0f) * angle + (ambLight.g / 255.0f) * (h.lastShape->c.g / 255.0f)) * 255.0f; //green
	rgb[2] = ((dLight.r / 255.0f) * (h.lastShape->c.b / 255.0f) * angle + (ambLight.b / 255.0f) * (h.lastShape->c.b / 255.0f)) * 255.0f; //blue

	for (float &i : rgb) //Make sure colors don't fall out of range and become "too bright"
	{
		if (i > 255.0f)
		{
			i = 255.0f;
		}
	}

	Color result(rgb[0], rgb[1], rgb[2]);

	return result;
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

	if (pow(b, 2) - w > 0) //Make sure we have no imaginary roots
	{
		t[0] = -b + sqrt(pow(b, 2) - w);
		t[1] = -b - sqrt(pow(b, 2) - w);
	}
	else
	{
		return;
	}
	
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
	if (hit.t == -1 && min_t > 0 || min_t < hit.t && min_t > 0)
	{
		hit.t = min_t;
		hit.lastShape = this;
		hit.color = c;
		hit.lastNormal = normal(ray.o + ray.d*min_t); //Save normal
	}
}

Vec PSphere::normal(Vec& point)
{
	//Find vector from center to point on surface and normalize it
	Vec a  = point - center;
	a.Normalize();
	
	return a;
}

Color PSphere::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	Vec intersectionPoint = r.o + r.d*h.t;
	Vec n = normal(intersectionPoint);
	Vec lightDir = (light - intersectionPoint);
	lightDir.Normalize();
	float angle = n.Dot(lightDir);

	Color dLight = { 255, 255, 255 };
	Color ambLight = { 50, 50, 50 };

	float rgb[3];
	
	rgb[0] = ((dLight.r / 255.0f) * (h.lastShape->c.r / 255.0f) * angle + (ambLight.r / 255.0f) * (h.lastShape->c.r / 255.0f)) * 255.0f; //red
	rgb[1] = ((dLight.g / 255.0f) * (h.lastShape->c.g / 255.0f) * angle + (ambLight.g / 255.0f) * (h.lastShape->c.g / 255.0f)) * 255.0f; //green
	rgb[2] = ((dLight.r / 255.0f) * (h.lastShape->c.b / 255.0f) * angle + (ambLight.b / 255.0f) * (h.lastShape->c.b / 255.0f)) * 255.0f; //blue

	for (float &i : rgb) //Make sure colors don't fall out of range and become "too bright"
	{
		if (i > 255.0f)
		{
			i = 255.0f;
		}
	}

	return Color (rgb[0], rgb[1], rgb[2]);
}
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//PTriangle functions
//---------------------------------------------------------------------
PTriangle::PTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color)
{
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
	c = _color;

	//Create the two edges
	edge0 = p2 - p1;
	edge1 = p3 - p1;
}

void PTriangle::test(Ray& ray, HitData& hit)
{
	///u and v, barycentric coordinates
	///w = (1 - u - v)
	///r(t) = f(u, v)
	///o + td = (1 - u - v)p1 + up2 + vp3
	float epsilon = 0.000001f;
	float t = 0.0f;

	Vec q = cross(ray.d, edge1);
	float a = edge0.Dot(q);

	if (a > -epsilon && a < epsilon)
		return;

	float f = 1 / a;
	Vec s = ray.o - p1;
	float u = f * (s.Dot(q));

	if (u < 0.0f)
		return;

	Vec r = cross(s, edge0);
	float v = f * (ray.d.Dot(r));

	if (v < 0.0f || u + v > 1.0f)
		return;

	t = f * (edge1.Dot(r));
	
	if (hit.t == -1 && t > 0 || t < hit.t && t > 0) //Initial value stored in hit.t if t is larger than 0
	{
		hit.t = t;
		hit.lastShape = this;
		hit.color = c;
		hit.lastNormal = normal(ray.o + ray.d*t); //Save normal
	}
}

Vec PTriangle::normal(Vec& point)
{
	//Calculate normal with cross product
	nor = cross(edge0, edge1);
	nor.Normalize();
	return nor;
}

Color PTriangle::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	
	Vec intersectionPoint = r.o + r.d*h.t;
	Vec n = normal(intersectionPoint);
	Vec lightDir = (light - intersectionPoint);
	lightDir.Normalize();
	float angle = n.Dot(lightDir);

	Color dLight = { 255, 255, 255 };
	Color ambLight = { 50, 50, 50 };

	float rgb[3];

	//Calculate the strenght of each color channel
	rgb[0] = ((dLight.r / 255.0f) * (h.lastShape->c.r / 255.0f) * angle + (ambLight.r / 255.0f) * (h.lastShape->c.r / 255.0f)) * 255.0f; //red
	rgb[1] = ((dLight.g / 255.0f) * (h.lastShape->c.g / 255.0f) * angle + (ambLight.g / 255.0f) * (h.lastShape->c.g / 255.0f)) * 255.0f; //green
	rgb[2] = ((dLight.r / 255.0f) * (h.lastShape->c.b / 255.0f) * angle + (ambLight.b / 255.0f) * (h.lastShape->c.b / 255.0f)) * 255.0f; //blue

	//Prevent color values from going over 255
	for (float &i : rgb)
	{
		if (i > 255.0f)
		{
			i = 255.0f;
		}
	}

	//Assemble the final color and return it
	Color result(rgb[0], rgb[1], rgb[2]);

	return result;
	
}
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//POBB functions
//---------------------------------------------------------------------

POBB::POBB(Vec b, Vec b1, Vec b2, Vec b3, float Hu, float Hv, float Hw, Color _color)
{
	Bcenter = b;
	Bu = b1;
	Bv = b2;
	Bw = b3;

	halfBu = Hu;
	halfBv = Hv;
	halfBw = Hw;

	c = _color;
}

POBB::POBB(Vec b, float Hu, float Hv, float Hw, Color _color)
{
	Bcenter = b;

	halfBu = Hu;
	halfBv = Hv;
	halfBw = Hw;
	
	c = _color;
}

void POBB::test(Ray& ray, HitData& hit)
{

	float epsilon = 0.000001;
	float tmin = -INFINITY;
	float tmax = INFINITY;
	Vec p = Bcenter - ray.o;

	int hcounter = -1; //Dumb way of doing things but it (supposedly) works
	float harr[3] = {halfBu, halfBv, halfBw};

	for (Vec i : {Bu, Bv, Bw})
	{
		float t1, t2;

		hcounter++;

		float e = i.Dot(p);
		float f = i.Dot(ray.d);
		if (abs(f) > epsilon)
		{
			t1 = (e + harr[hcounter]) / f;
			t2 = (e - harr[hcounter]) / f;

			if (t1 > t2) { std::swap(t1, t2); }

			if (t1 > tmin) { tmin = t1; }
			if (t2 < tmax) { tmax = t2; }

			if (tmin > tmax)
				return;
			if (tmax < 0)
				return;
		}
		else if (-e - harr[hcounter] > 0 || -e + harr[hcounter] < 0)
			return;
	}

	if (tmin > 0)
	{
		if (hit.t == -1 && tmin > 0 || tmin < hit.t && tmin > 0) //Initial value stored in hit.t if t is larger than 0
		{
			hit.t = tmin;
			hit.lastShape = this;
			hit.color = c;
			hit.lastNormal = normal(ray.o + ray.d*tmin); //Save normal
		}
	}
	else
	{
		if (hit.t == -1 && tmax > 0 || tmax < hit.t && tmax > 0) //Initial value stored in hit.t if t is larger than 0
		{
			hit.t = tmax;
			hit.lastShape = this;
			hit.color = c;
			hit.lastNormal = normal(ray.o + ray.d*tmax); //Save normal
		}
	}
}

Vec POBB::normal(Vec& point)
{
	Vec baseArr[6] = {Bu, Bv, Bw, Bu*-1.0f, Bv*-1.0f, Bw*-1.0f};
	float halfArr[3] = { halfBu, halfBv, halfBw};
	int halfArrC = 0;

	Vec result = { 0, 0, 0 };

	for (Vec &i : baseArr)
	{

		Vec s = Bcenter + (i*halfArr[halfArrC]);
		
		float HmS_N_dot = (point - s).Dot(i);

		if (abs(HmS_N_dot) < 0.001f)
		{
			result = i;
		}

		///Reset counter
		if (halfArrC < 2)
			halfArrC++;
		else
			halfArrC = 0;
	}

	return result;
}

Color POBB::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	Vec intersectionPoint = r.o + r.d*h.t;
	Vec n = normal(intersectionPoint);
	Vec lightDir = (light - intersectionPoint);
	lightDir.Normalize();
	float angle = n.Dot(lightDir);

	Color dLight = { 255, 255, 255 };
	Color ambLight = { 50, 50, 50 };

	float rgb[3];

	rgb[0] = ((dLight.r / 255.0f) * (h.lastShape->c.r / 255.0f) * angle + (ambLight.r / 255.0f) * (h.lastShape->c.r / 255.0f)) * 255.0f; //red
	rgb[1] = ((dLight.g / 255.0f) * (h.lastShape->c.g / 255.0f) * angle + (ambLight.g / 255.0f) * (h.lastShape->c.g / 255.0f)) * 255.0f; //green
	rgb[2] = ((dLight.r / 255.0f) * (h.lastShape->c.b / 255.0f) * angle + (ambLight.b / 255.0f) * (h.lastShape->c.b / 255.0f)) * 255.0f; //blue

	for (float &i : rgb) //Make sure colors don't fall out of range and become "too bright"
	{
		if (i > 255.0f)
		{
			i = 255.0f;
		}
	}

	Color result(rgb[0], rgb[1], rgb[2]);

	return result;
}

//---------------------------------------------------------------------

//Cross product
Vec cross(Vec A, Vec B)
{
	float x = (A.y * B.z) - (A.z * B.y);
	float y = (A.z * B.x) - (A.x * B.z);
	float z = (A.x * B.y) - (A.y * B.x);

	Vec result(x, y, z);
	return result;
}

