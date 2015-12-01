#ifndef INCLUDED_SGE_MATH_H
#define INCLUDED_SGE_MATH_H

//====================================================================================================
// Filename:	SGE_Math.h
// Description:	Header containing some useful math structures and helper functions.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <float.h>
#include <math.h>

#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Constants
//====================================================================================================

namespace MathConsts
{
	static const float kPI		= 3.1415926535f;
	static const float kEpsilon	= 0.000001f;
};

//====================================================================================================
// Structs
//====================================================================================================

struct Vector2
{
	float x;
	float y;

	// Constructors
	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2& v);

	// Helper functions
	void Invert();
	void PerpendicularLH();
	void PerpendicularRH();
	float Length() const;
	float LengthSquared() const;
	void Normalize();
	void Truncate(float max);
	void Rotate(float rad);
	void Reflect(const Vector2& normal);

	// Overloaded operators
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(float f) const;
	Vector2 operator/(float f) const;

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float f);
	Vector2& operator/=(float f);

	Vector2& operator=(const Vector2& v);
	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;
};

//----------------------------------------------------------------------------------------------------

struct Matrix33
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	// Constructors
	Matrix33();
	Matrix33(const Matrix33& v);

	// Helper functions
	void Identity();
	void SetTranslate(float x, float y);
	void SetRotate(float rad);
	void SetRotate(const Vector2& dir, const Vector2& side);
	void SetScale(float x, float y);
	float Determinant();
	void Multiply(float scale);
	void Multiply(const Matrix33& mat);
	void Inverse();
	Vector2 TransformCoord(const Vector2& v);
	Vector2 TransformNormal(const Vector2& v);
};

//----------------------------------------------------------------------------------------------------

struct LineSegment
{
	Vector2 from;
	Vector2 to;

	// Constructors
	LineSegment();
	LineSegment(float x0, float y0, float x1, float y1);
	LineSegment(const Vector2& v0, const Vector2& v1);
};

//----------------------------------------------------------------------------------------------------

struct Circle
{
	Vector2 center;
	float radius;

	// Constructors
	Circle();
	Circle(float _x, float _y, float r);
	Circle(const Vector2& c, float r);
};

//----------------------------------------------------------------------------------------------------

struct Rect
{
	Vector2 min;
	Vector2 max;

	// Constructors
	Rect();
	Rect(float l, float t, float r, float b);
	Rect(const Vector2& _min, const Vector2& _max);

	// Helper functions
	void Expand(const Vector2& point);

	bool IsValid() const					{ return (min.x < max.x) && (min.y < max.y); }

	LineSegment GetLeftSegment() const		{ return LineSegment(min.x, min.y, min.x, max.y); }
	LineSegment GetRightSegment() const	{ return LineSegment(max.x, min.y, max.x, max.y); }
	LineSegment GetTopSegment() const		{ return LineSegment(min.x, min.y, max.x, min.y); }
	LineSegment GetBottomSegment() const	{ return LineSegment(min.x, max.y, max.x, max.y); }

	float GetWidth() const					{ return max.x - min.x; }
	float GetHeight() const					{ return max.y - min.y; }

	// Overloaded operators
	Rect operator+(const Vector2& v) const;
	Rect operator+(const Rect& r) const;

	Rect& operator+=(const Vector2& v);
	Rect& operator+=(const Rect& r);
};

//====================================================================================================
// Helper Functions
//====================================================================================================

// Common math functions
float Min(float a, float b);
float Max(float a, float b);
float Clamp(float f, float min, float max);

// Functions to convert between degrees and radians
float Math_DegToRad(float degree);
float Math_RadToDeg(float radian);

// Function to return the unit vector
Vector2 Math_Normalize(const Vector2& v);

// Function to return the dot product
float Math_Dot(const Vector2& v0, const Vector2& v1);

// Functions to return the length of a vector
float Math_Length(const Vector2& v);
float Math_LengthSquared(const Vector2& v);

// Functions to calculate the distance between two points
float Math_Distance(const Vector2& v0, const Vector2& v1);
float Math_DistanceSquared(const Vector2& v0, const Vector2& v1);

// Function to check if a point is inside a rectangle
bool Math_PointInRect(const Vector2& point, const Rect& rect);
bool Math_PointInCircle(const Vector2& point, const Circle& circle);

// Functions for intersect tests between different shapes
bool Math_Intersect(const LineSegment& a, const LineSegment& b);
bool Math_Intersect(const Circle& c0, const Circle& c1);
bool Math_Intersect(const Rect& r0, const Rect& r1);

bool Math_Intersect(const LineSegment& l, const Circle& c);
bool Math_Intersect(const Circle& c, const LineSegment& l);

bool Math_Intersect(const Circle& c, const Rect& r);
bool Math_Intersect(const Rect& r, const Circle& c);

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_MATH_H