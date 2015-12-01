//====================================================================================================
// Filename:	SGE_Math.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SGE_Math.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Structs
//====================================================================================================

Vector2::Vector2()
	: x(0.0f)
	, y(0.0f)
{}

//----------------------------------------------------------------------------------------------------

Vector2::Vector2(float _x, float _y)
	: x(_x)
	, y(_y)
{}

//----------------------------------------------------------------------------------------------------

Vector2::Vector2(const Vector2& v)
	: x(v.x)
	, y(v.y)
{}

//----------------------------------------------------------------------------------------------------

void Vector2::Invert()
{
	x = -x;
	y = -y;
}

//----------------------------------------------------------------------------------------------------

void Vector2::PerpendicularLH()
{
	float temp = x;
	x = -y;
	y = temp;
}

//----------------------------------------------------------------------------------------------------

void Vector2::PerpendicularRH()
{
	float temp = x;
	x = y;
	y = -temp;
}

//----------------------------------------------------------------------------------------------------

float Vector2::Length() const
{
	return sqrt((x * x) + (y * y));
}

//----------------------------------------------------------------------------------------------------

float Vector2::LengthSquared() const
{
	return ((x * x) + (y * y));
}

//----------------------------------------------------------------------------------------------------

void Vector2::Normalize()
{
	float mag = Length();
	if (mag != 0.0f)
	{
		x /= mag;
		y /= mag;
	}
}

//----------------------------------------------------------------------------------------------------

void Vector2::Truncate(float max)
{
	const float len = Length();
	if (len > MathConsts::kEpsilon)
	{
		const float ratio = max / len;
		if (ratio < 1.0f)
		{
			x *= ratio;
			y *= ratio;
		}
	}
}

//----------------------------------------------------------------------------------------------------

void Vector2::Reflect(const Vector2& normal)
{
	const Vector2 temp = normal * Math_Dot(*this, normal);
	x = x - 2.0f * temp.x;
	y = y - 2.0f * temp.y;
}

//----------------------------------------------------------------------------------------------------

void Vector2::Rotate(float rad)
{
	const float kCosAngle = cos(rad);
	const float kSinAngle = sin(rad);

	Vector2 temp(x, y);
	x = temp.x * kCosAngle - temp.y * kSinAngle;
	y = temp.y * kCosAngle + temp.x * kSinAngle;
}

//----------------------------------------------------------------------------------------------------

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

//----------------------------------------------------------------------------------------------------

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}

//----------------------------------------------------------------------------------------------------

Vector2 Vector2::operator*(float f) const
{
	return Vector2(x * f, y * f);
}

//----------------------------------------------------------------------------------------------------

Vector2 Vector2::operator/(float f) const
{
	const float inv = 1.0f / f;
	return Vector2(x * inv, y * inv);
}

//----------------------------------------------------------------------------------------------------

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

Vector2& Vector2::operator*=(float f)
{
	x *= f;
	y *= f;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

Vector2& Vector2::operator/=(float f)
{
	const float inv = 1.0f / f;
	x *= inv;
	y *= inv;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

Vector2& Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

bool Vector2::operator==(const Vector2& v) const
{
	return (fabs(x - v.x) < MathConsts::kEpsilon && fabs(y - v.y) < MathConsts::kEpsilon);
}

//----------------------------------------------------------------------------------------------------

bool Vector2::operator!=(const Vector2& v) const
{
	return (x > v.x || x < v.x || y > v.y || y < v.y);
}

//----------------------------------------------------------------------------------------------------

Matrix33::Matrix33()
	: _11(1.0f), _12(0.0f), _13(0.0f)
	, _21(0.0f), _22(1.0f), _23(0.0f)
	, _31(0.0f), _32(0.0f), _33(1.0f)
{}

//----------------------------------------------------------------------------------------------------

Matrix33::Matrix33(const Matrix33& v)
	: _11(v._11), _12(v._12), _13(v._13)
	, _21(v._21), _22(v._22), _23(v._23)
	, _31(v._31), _32(v._32), _33(v._33)
{}

//----------------------------------------------------------------------------------------------------

void Matrix33::Identity()
{
	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f;
	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f;
	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f;
}

//----------------------------------------------------------------------------------------------------

void Matrix33::SetTranslate(float x, float y)
{
	_11 = 1.0f;	_12 = 0.0f;	_13 = 0.0f;
	_21 = 0.0f;	_22 = 1.0f;	_23 = 0.0f;
	_31 = x;	_32 = y;	_33 = 1.0f;
}

//----------------------------------------------------------------------------------------------------

void Matrix33::SetRotate(float rad)
{
	const float c = cos(rad);
	const float s = sin(rad);

	_11 = c;	_12 = s;	_13 = 0.0f;
	_21 = -s;	_22 = c;	_23 = 0.0f;
	_31 = 0.0f;	_32 = 0.0f;	_33 = 1.0f;
}

//----------------------------------------------------------------------------------------------------

void Matrix33::SetRotate(const Vector2& dir, const Vector2& side)
{
	_11 = dir.x;	_12 = dir.y;	_13 = 0.0f;
	_21 = side.x;	_22 = side.y;	_23 = 0.0f;
	_31 = 0.0f;		_32 = 0.0f;		_33 = 1.0f;
}

//----------------------------------------------------------------------------------------------------

void Matrix33::SetScale(float x, float y)
{
	_11 = x;	_12 = 0.0f;	_13 = 0.0f;
	_21 = 0.0f;	_22 = y;	_23 = 0.0f;
	_31 = 0.0f;	_32 = 0.0f;	_33 = 1.0f;
}

//----------------------------------------------------------------------------------------------------

float Matrix33::Determinant()
{
	const float A = _11 * (_22 * _33 - _23 * _32);
	const float B = _12 * (_21 * _33 - _23 * _31);
	const float C = _13 * (_21 * _32 - _22 * _31);
	return (A - B + C);
}

//----------------------------------------------------------------------------------------------------

void Matrix33::Multiply(float scale)
{
	_11 *= scale; _12 *= scale; _13 *= scale;
	_21 *= scale; _22 *= scale; _23 *= scale;
	_31 *= scale; _32 *= scale; _33 *= scale;
}

//----------------------------------------------------------------------------------------------------

void Matrix33::Multiply(const Matrix33& mat)
{
	Matrix33 temp(*this);

	_11 = (temp._11 * mat._11) + (temp._12 * mat._21) + (temp._13 * mat._31);
	_12 = (temp._11 * mat._12) + (temp._12 * mat._22) + (temp._13 * mat._32);
	_13 = (temp._11 * mat._13) + (temp._12 * mat._23) + (temp._13 * mat._33);

	_21 = (temp._21 * mat._11) + (temp._22 * mat._21) + (temp._23 * mat._31);
	_22 = (temp._21 * mat._12) + (temp._22 * mat._22) + (temp._23 * mat._32);
	_23 = (temp._21 * mat._13) + (temp._22 * mat._23) + (temp._23 * mat._33);

	_31 = (temp._31 * mat._11) + (temp._32 * mat._21) + (temp._33 * mat._31);
	_32 = (temp._31 * mat._12) + (temp._32 * mat._22) + (temp._33 * mat._32);
	_33 = (temp._31 * mat._13) + (temp._32 * mat._23) + (temp._33 * mat._33);
}

//----------------------------------------------------------------------------------------------------

void Matrix33::Inverse()
{
	Matrix33 temp(*this);
	const float invDet = 1.0f / Determinant();

	_11 = (temp._22 * temp._33 - temp._23 * temp._32) * invDet;
	_12 = (temp._13 * temp._32 - temp._12 * temp._33) * invDet;
	_13 = (temp._12 * temp._23 - temp._13 * temp._22) * invDet;

	_21 = (temp._23 * temp._31 - temp._21 * temp._33) * invDet;
	_22 = (temp._11 * temp._33 - temp._13 * temp._31) * invDet;
	_23 = (temp._13 * temp._21 - temp._11 * temp._23) * invDet;

	_31 = (temp._21 * temp._32 - temp._22 * temp._31) * invDet;
	_32 = (temp._12 * temp._31 - temp._11 * temp._32) * invDet;
	_33 = (temp._11 * temp._22 - temp._12 * temp._21) * invDet;
}

//----------------------------------------------------------------------------------------------------

Vector2 Matrix33::TransformCoord(const Vector2& v)
{
	return Vector2((v.x * _11) + (v.y * _21) + (1.0f * _31),
					(v.x * _12) + (v.y * _22) + (1.0f * _32));
}

//----------------------------------------------------------------------------------------------------

Vector2 Matrix33::TransformNormal(const Vector2& v)
{
	return Vector2((v.x * _11) + (v.y * _21), (v.x * _12) + (v.y * _22));
}

//----------------------------------------------------------------------------------------------------

LineSegment::LineSegment()
	: from(0.0f, 0.0f)
	, to(0.0f, 0.0f)
{}

//----------------------------------------------------------------------------------------------------

LineSegment::LineSegment(float x0, float y0, float x1, float y1)
	: from(x0, y0)
	, to(x1, y1)
{}

//----------------------------------------------------------------------------------------------------

LineSegment::LineSegment(const Vector2& v0, const Vector2& v1)
	: from(v0)
	, to(v1)
{}

//----------------------------------------------------------------------------------------------------

Circle::Circle()
	: center(0.0f, 0.0f)
	, radius(0.0f)
{}

//----------------------------------------------------------------------------------------------------

Circle::Circle(float _x, float _y, float r)
	: center(_x, _y)
	, radius(r)
{}

//----------------------------------------------------------------------------------------------------

Circle::Circle(const Vector2& c, float r)
	: center(c)
	, radius(r)
{}

//----------------------------------------------------------------------------------------------------

Rect::Rect()
	: min(FLT_MAX, FLT_MAX)
	, max(-FLT_MAX, -FLT_MAX)
{}

//----------------------------------------------------------------------------------------------------

Rect::Rect(float l, float t, float r, float b)
	: min(l, t)
	, max(r, b)
{}

//----------------------------------------------------------------------------------------------------

Rect::Rect(const Vector2& _min, const Vector2& _max)
	: min(_min)
	, max(_max)
{}

//----------------------------------------------------------------------------------------------------

void Rect::Expand(const Vector2& point)
{
	min.x = Min(min.x, point.x);
	min.y = Min(min.y, point.y);
	max.x = Max(max.x, point.x);
	max.y = Max(max.y, point.y);
}

//----------------------------------------------------------------------------------------------------

Rect Rect::operator+(const Vector2& v) const
{
	return Rect(min + v, max + v);
}

//----------------------------------------------------------------------------------------------------

Rect Rect::operator+(const Rect& r) const
{
	return Rect
	(
		Min(min.x, r.min.x),
		Min(min.y, r.min.y),
		Max(max.x, r.max.x),
		Max(max.y, r.max.y)
	);
}

//----------------------------------------------------------------------------------------------------

Rect& Rect::operator+=(const Vector2& v)
{
	min += v;
	max += v;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

Rect& Rect::operator+=(const Rect& r)
{
	min.x = Min(min.x, r.min.x);
	min.y = Min(min.y, r.min.y);
	max.x = Max(max.x, r.max.x);
	max.y = Max(max.y, r.max.y);
	return (*this);
}

//====================================================================================================
// Helper Functions
//====================================================================================================

float Min(float a, float b)
{
	return (a <= b) ? a : b;
}

//----------------------------------------------------------------------------------------------------

float Max(float a, float b)
{
	return (a > b) ? a : b;
}

//----------------------------------------------------------------------------------------------------

float Clamp(float f, float min, float max)
{
	float result = f;
	if (f < min)
	{
		result = min;
	}
	if (f > max)
	{
		result = max;
	}
	return result;
}

//----------------------------------------------------------------------------------------------------

float Math_DegToRad(float degree)
{
	return degree * MathConsts::kPI / 180.0f;
}

//----------------------------------------------------------------------------------------------------

float Math_RadToDeg(float radian)
{
	return radian * 180.0f / MathConsts::kPI;
}

//----------------------------------------------------------------------------------------------------

Vector2 Math_Normalize(const Vector2& v)
{
	Vector2 norm(v);
	norm.Normalize();
	return norm;
}

//----------------------------------------------------------------------------------------------------

float Math_Dot(const Vector2& v0, const Vector2& v1)
{
	return (v0.x * v1.x) + (v0.y * v1.y);
}

//----------------------------------------------------------------------------------------------------

float Math_Length(const Vector2& v)
{
	return v.Length();
}

//----------------------------------------------------------------------------------------------------

float Math_LengthSquared(const Vector2& v)
{
	return v.LengthSquared();
}

//----------------------------------------------------------------------------------------------------

float Math_Distance(const Vector2& v0, const Vector2& v1)
{
	float fDiffX = v0.x - v1.x;
	float fDiffY = v0.y - v1.y;

	return sqrt(fDiffX * fDiffX + fDiffY * fDiffY);
}

//----------------------------------------------------------------------------------------------------

float Math_DistanceSquared(const Vector2& v0, const Vector2& v1)
{
	float fDiffX = v0.x - v1.x;
	float fDiffY = v0.y - v1.y;

	return (fDiffX * fDiffX + fDiffY * fDiffY);
}

//----------------------------------------------------------------------------------------------------

bool Math_PointInRect(const Vector2& point, const Rect& rect)
{
	if (point.x > rect.max.x || point.x < rect.min.x ||
		point.y > rect.max.y || point.y < rect.min.y)
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------

bool Math_PointInCircle(const Vector2& point, const Circle& circle)
{
	const Vector2 centerToPoint = point - circle.center;
	const float distSqr = Math_Dot(centerToPoint, centerToPoint);
	const float radiusSqr = circle.radius * circle.radius;
	return distSqr < radiusSqr;
}

//----------------------------------------------------------------------------------------------------

bool Math_Intersect(const LineSegment& a, const LineSegment& b)
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------

bool Math_Intersect(const Circle& c0, const Circle& c1)
{
	const float fRadii = c0.radius + c1.radius;
	const float fRadiiSquared = fRadii * fRadii;
	const float fDistanceSquared = Math_DistanceSquared(c0.center, c1.center);
	return (fDistanceSquared < fRadiiSquared);
}

//----------------------------------------------------------------------------------------------------

bool Math_Intersect(const Rect& r0, const Rect& r1)
{
	if (!r0.IsValid() || !r1.IsValid())
		return false;
	else if (r0.min.x > r1.max.x)
		return false;
	else if (r0.min.y > r1.max.y)
		return false;
	else if (r0.max.x < r1.min.x)
		return false;
	else if (r0.max.y < r1.min.y)
		return false;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool Math_Intersect(const LineSegment& l, const Circle& c)
{
	return Math_Intersect(c, l);
}

//----------------------------------------------------------------------------------------------------

bool Math_Intersect(const Circle& c, const LineSegment& l)
{
	Vector2 startToCenter = c.center - l.from;
	Vector2 startToEnd = l.to - l.from;
	float len = startToEnd.Length();
	Vector2 dir = startToEnd / len;

	// Find the closest point to the line segment
	float projection = Math_Dot(startToCenter, dir);
	Vector2 closestPoint;
	if (projection > len)
	{
		closestPoint = l.to;
	}
	else if (projection < 0.0f)
	{
		closestPoint = l.from;
	}
	else
	{
		closestPoint = l.from + (dir * projection);
	}

	// Check if the closest point is within the circle
	Vector2 closestToCenter = c.center - closestPoint;
	if (closestToCenter.LengthSquared() > c.radius * c.radius)
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------

bool Math_Intersect(const Circle& c, const Rect& r)
{
	return Math_Intersect(r, c);
}

//----------------------------------------------------------------------------------------------------

bool Math_Intersect(const Rect& r, const Circle& c)
{
	if (!r.IsValid())
	{
		return false;
	}

	Vector2 closestPoint;
	closestPoint.x = Clamp(c.center.x, r.min.x, r.max.x);
	closestPoint.y = Clamp(c.center.y, r.min.y, r.max.y);
	
	const float distance = Math_Distance(closestPoint, c.center);
	if (distance > c.radius)
	{
		return false;
	}
	return true;
}

} // namespace SGE