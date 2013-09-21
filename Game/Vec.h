//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

#include "Vec2.h"

//////////////////////////////////////////////////////////////////////

struct Vec3;

__declspec(align(16)) struct Vector
{
	enum
	{
		X = 0,
		Y = 1,
		Z = 2,
		W = 3
	};

	template <uint32 a, uint32 b, uint32 c, uint32 d> static __m128 Permute(__m128 const v)
	{
		return _mm_shuffle_ps(v, v, (d << 6) | (c << 4) | (b << 2) | a);
	}

	template <uint32 a, uint32 b, uint32 c, uint32 d> static __m128 Permute(__m128 const c1, __m128 const c2)
	{
		return _mm_shuffle_ps(c1, c2, (d << 6) | (c << 4) | (b << 2) | a);
	}

	union
	{
		__m128 m;
		float f[4];
		struct
		{
			float x, y, z, w;
		};
	};

	Vector()
	{
	}

	~Vector()
	{
	}

	Vector(__m128 v)
		: m(v)
	{
	}

	Vector(Vec3 v);

	operator __m128()
	{
		return m;
	}

	Vector(float x, float y, float z)
	{
		m = _mm_set_ps(0.0f, z, y, x);
	}

	Vector(float x, float y, float z, float w)
	{
		m = _mm_set_ps(w, z, y, x);
	}

	Vector Dot(__m128 b) const
	{
		__m128 dot = _mm_mul_ps(m, b);
		__m128 temp = Permute<2,1,2,1>(dot);
		dot = _mm_add_ss(dot, temp);
		temp = Permute<1,1,1,1>(temp);
		dot = _mm_add_ss(dot, temp);
		return Permute<0,0,0,0>(dot);
	}

	Vector LengthSquared() const
	{
		return Dot(m);
	}

	float Length() const
	{
		return sqrtf(Dot(m).x);
	}

	template <int x, int y, int z, int w> static Vector Shuffle(__m128 v)
	{
		return _mm_shuffle_ps(v, v, (w << 6) | (z << 4) | (y << 2) | x);
	}
};

//////////////////////////////////////////////////////////////////////

inline __m128 operator + (__m128 a, __m128 b)
{
	return _mm_add_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline __m128 operator - (__m128 a, __m128 b)
{
	return _mm_sub_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline __m128 operator * (__m128 a, __m128 b)
{
	return _mm_mul_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline __m128 operator / (__m128 a, __m128 b)
{
	return _mm_div_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline __m128 operator * (__m128 a, float b)
{
	return _mm_mul_ps(a, _mm_set_ps(b, b, b, b));
}

//////////////////////////////////////////////////////////////////////

inline __m128 operator / (__m128 a, float b)
{
	return _mm_div_ps(a, _mm_set_ps(b, b, b, b));
}

//////////////////////////////////////////////////////////////////////

struct Vec3
{
	float x;
	float y;
	float z;

	//////////////////////////////////////////////////////////////////////

	Vec3()
	{
	}

	//////////////////////////////////////////////////////////////////////

	Vec3(float _x, float _y, float _z)
		: x(_x)
		, y(_y)
		, z(_z)

	{
	}

	//////////////////////////////////////////////////////////////////////

	Vec3(__m128 m)
	{
		Vector v(m);
		x = v.x;
		y = v.y;
		z = v.z;
	}

	//////////////////////////////////////////////////////////////////////

	Vec3(btVector3 const &bt)
		: x(bt.x())
		, y(bt.y())
		, z(bt.z())
	{
	}

	//////////////////////////////////////////////////////////////////////

	float LengthSquared() const throw()
	{
		return x * x + y * y + z * z;
	}

	//////////////////////////////////////////////////////////////////////

	float Length() const throw()
	{
		return sqrtf(LengthSquared());
	}

	//////////////////////////////////////////////////////////////////////

	Vec3 &Normalize() throw()
	{
		float l = 1.0f / Length();
		x *= l;
		y *= l;
		z *= l;
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////

inline Vec3 operator - (Vec3 const &a, Vec3 const &b)
{
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

//////////////////////////////////////////////////////////////////////

inline Vec3 operator + (Vec3 const &a, Vec3 const &b)
{
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

//////////////////////////////////////////////////////////////////////

inline Vec3 &operator += (Vec3 &a, Vec3 const &b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vec3 &operator -= (Vec3 &a, Vec3 const &b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vec3 operator * (Vec3 const &a, float b)
{
	return Vec3(a.x * b, a.y * b, a.z * b);
}

//////////////////////////////////////////////////////////////////////

inline Vec3 operator / (Vec3 const &a, float b)
{
	return Vec3(a.x / b, a.y / b, a.z / b);
}

//////////////////////////////////////////////////////////////////////

inline float Dot(Vec3 const &a, Vec3 const &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//////////////////////////////////////////////////////////////////////

inline Vec3 Cross(Vec3 const &a, Vec3 const &b)
{
	return Vec3(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
}

//////////////////////////////////////////////////////////////////////

inline Vector::Vector(Vec3 v)
{
	m = _mm_set_ps(0.0f, v.z, v.y, v.x);
}
