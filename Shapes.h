#include <math.h>
#include <memory.h>
//#include <assert.h>
#include <algorithm>

typedef unsigned char BYTE;

struct Vec
{
	float	x, y, z;
	Vec() {};
	Vec(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	float Dot(const Vec& v2) { return (x * v2.x) + (y * v2.y) + (z * v2.z); }

	Vec operator-(const Vec& v)
	{
		return Vec(this->x - v.x, this->y - v.y, this->z - v.z);
	}

	Vec operator+(const Vec& v)
	{
		return Vec(this->x + v.x, this->y + v.y, this->z + v.z);
	}

	Vec operator*(const float& f)
	{
		return Vec(this->x * f, this->y * f, this->z * f);
	}

	// useful when comparing lengths! (faster)
	inline float Length2()
	{
		return (x*x + y*y + z*z);
	}

	float Length()
	{
		return sqrt(Length2());
	}

	void Normalize()
	{
		float l = Length();
		x /= l;
		y /= l;
		z /= l;
	}
};

struct Ray
{
	Vec	o;
	Vec	d;

	Ray(Vec origin, Vec direction) : o(origin), d(direction) { };
};

struct Color
{
	BYTE	r;
	BYTE	g;
	BYTE	b;

	Color() { memset(&r, 0, 3); }
	Color(BYTE red, BYTE green, BYTE blue) : r(red), g(green), b(blue) {};
	const Color& operator+(const Color& plus) {
		unsigned int rr, gg, bb;
		rr = plus.r + r; 
		if (rr > 255) 
			rr = 255;
		gg = plus.g + g; 
		if (gg > 255) 
			gg= 255;
		bb = plus.b + b; 
		if (bb > 255) 
			bb = 255;
		return Color(rr,gg,bb);
	}
};

///My own crossproduct method
Vec cross(Vec A, Vec B);

class Shape;
struct HitData
{
	float	t;
	Color	color;
	Shape* lastShape = nullptr;
	Vec lastNormal;
	// hit data is -1 (not hit) and black (default color for background)
	HitData() : t(-1.0f), color(0, 0, 0) { };
};

class Shape
{
public:
	Color	c;
	// each subclass, has to implement these two methods!
	virtual void test(Ray& ray, HitData& hit) = 0;
	virtual Vec normal(Vec &point) = 0;

	// Light with (0.5,0.5,0.5) intensity for ambient, diffuse and specular
	// This method is the same for all clases, so implement only for Shape
	virtual Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);
};

///My own subclasses of Shape
class PPlane : public Shape
{
private:
	Vec n; //Normal
	float d; //Constant

public:
	void test(Ray& ray, HitData& hit); //Hit test
	Vec normal(Vec &point); //Returns the normal
	Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);

	PPlane(Vec normal, float _d, Color color); //Constructor
};

class PSphere : public Shape
{
private:
	Vec center;
	float radius;
	float radius2;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
	Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);

	PSphere(Vec _center, float _radius, Color _color);
};

class PTriangle : public Shape
{
private:
	Vec p1, p2, p3, nor;
	Vec edge0, edge1;

public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
	Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);

	PTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color);
};

class POBB : public Shape
{
private:
	Vec Bcenter;
	Vec Bu;
	Vec Bv;
	Vec Bw;

	Vec Pu, Puo;
	Vec Pv, Pvo;
	Vec Pw, Pwo;

	float halfBu;
	float halfBv;
	float halfBw;

	// Center point, lenght U vector, length V vector, length W vector, color

public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec& point);

	POBB(Vec b, Vec b1, Vec b2, Vec b3, float Hu, float Hv, float Hw, Color _color);
	POBB(Vec b, float Hu, float Hv, float Hw, Color _color);

	Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);
};

//--------------------------------------------------------------------
// Classes that came with the project
//--------------------------------------------------------------------

// for each class, add the necessary data that describes the shape,
// implement a constructor, and implement the test function
// EACH CLASS SHOULD INHERIT FROM SHAPE,
class MPlane : public Shape
{
	Vec		n;
	float	d;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
	MPlane(Vec normal, float _d, Color color);
};

class MSphere : public Shape
{
	Vec center;
	float radius;
	float radius2;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
	MSphere(Vec _center, float _radius, Color _color);
};

class MTriangle : public Shape
{
	Vec p1, p2, p3, nor;
	Vec edge0, edge1;
	public:
		void test(Ray& ray, HitData& hit);
		Vec normal(Vec &point) { return nor; }
		MTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color);

};

class MOBB : public Shape
{
public:
	Vec Bcenter;
	Vec Bu;
	Vec Bv;
	Vec Bw;

	Vec Pu,Puo;
	Vec Pv,Pvo;
	Vec Pw,Pwo;

	float halfBu;
	float halfBv;
	float halfBw;

	void test(Ray& ray, HitData& hit);
	Vec normal(Vec& point);

	// Center point, lenght U vector, length V vector, length W vector, color

	MOBB(Vec b, Vec b1, Vec b2, Vec b3, float Hu, float Hv, float Hw, Color _color);
	MOBB(Vec b, float Hu, float Hv, float Hw, Color _color);
};