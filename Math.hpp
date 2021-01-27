#pragma once
class Vector2
{
public:
	float x, y;

	inline Vector2()
		: x(0),
		y(0)
	{
	}


	inline Vector2(float X, float Y)
		: x(X),
		y(Y)
	{
	}
	inline Vector2& operator +=(const Vector2& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	inline Vector2& operator -=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}
	inline Vector2 operator -(const Vector2& v) const
	{
		return Vector2(x - v.x, y - v.y);
	}

	inline Vector2 operator +(const Vector2& v) const
	{
		return Vector2(x + v.x, y + v.y);
	}
	inline Vector2& operator *=(float v)
	{
		x *= v;
		y *= v;
		return *this;
	}
	inline void zero()
	{
		x = 0.0f;
		y = 0.0f;
	}
};

struct Vector3 {
	float x;
	float y;
	float z;
};