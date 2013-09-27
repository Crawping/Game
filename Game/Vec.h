//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

#include "Vec2.h"

//////////////////////////////////////////////////////////////////////

struct Vec3;

typedef __m128 Vec4;

__declspec(align(16)) struct Vec4i
{
	union
	{
		int32_t i[4];
		Vec4 v;
	};

	operator Vec4 () const
	{
		return v;
	}
};

extern const __declspec(selectany) Vec4i gMMaskXYZ = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };

#define Permute(d, c, b, a, v) _mm_shuffle_ps(v, v, ((d) << 6) | ((c) << 4) | ((b) << 2) | (a))
#define Permute2(d, c, b, a, v1, v2) _mm_shuffle_ps(v1, v2, ((d) << 6) | ((c) << 4) | ((b) << 2) | (a))


inline Vec4 Vec(float x, float y, float z)
{
	return _mm_set_ps(0.0f, z, y, x);
}

inline Vec4 Vec(float x, float y, float z, float w)
{
	return _mm_set_ps(w, z, y, x);
}

inline float X(Vec4 m)
{
	return _mm_cvtss_f32(m);
}

inline float Y(Vec4 m)
{
	return _mm_cvtss_f32(Permute(1,1,1,1, m));
}

inline float Z(Vec4 m)
{
	return _mm_cvtss_f32(Permute(2,2,2,2, m));
}

inline float W(Vec4 m)
{
	return _mm_cvtss_f32(Permute(3,3,3,3, m));
}

inline float Dot(Vec4 a, Vec4 b)
{
	Vec4 dot = _mm_mul_ps(a, b);
	Vec4 temp = Permute(2,1,2,1, dot);
	dot = _mm_add_ss(dot, temp);
	temp = Permute(1,1,1,1, temp);
	dot = _mm_add_ss(dot, temp);
	return X(dot);
}

inline float LengthSquared(Vec4 m)
{
	return Dot(m, m);
}

inline float Length(Vec4 m)
{
	Vec4 vLengthSq = _mm_mul_ps(m, m);
	Vec4 vTemp = Permute(2,1,2,1, vLengthSq);
	vLengthSq = _mm_add_ss(vLengthSq,vTemp);
	vTemp = Permute(1,1,1,1, vTemp);
	vLengthSq = _mm_add_ss(vLengthSq,vTemp);
	vLengthSq = Permute(0,0,0,0, vLengthSq);
	return X(_mm_sqrt_ps(vLengthSq));
}

inline Vec4 Normalize(Vec4 m)
{
	Vec4 vLengthSq = _mm_mul_ps(m, m);
	Vec4 vTemp = Permute(2,1,2,1, vLengthSq);
	vLengthSq = _mm_add_ss(vLengthSq,vTemp);
	vTemp = Permute(1,1,1,1, vTemp);
	vLengthSq = _mm_add_ss(vLengthSq,vTemp);
	vLengthSq = Permute(0,0,0,0, vLengthSq);
	return _mm_div_ps(m,_mm_sqrt_ps(vLengthSq));
}

inline Vec4 Cross(Vec4 a, Vec4 b)
{
	Vec4 t1 = Permute(3,0,2,1, a);
	Vec4 t2 = Permute(3,1,0,2, b);
	Vec4 r = _mm_mul_ps(t1, t2);
	t1 = Permute(3,0,2,1, t1);
	t2 = Permute(3,1,0,2, t2);
	t1 = _mm_mul_ps(t1, t2);
	r = _mm_sub_ps(r, t1);
	return _mm_and_ps(r, gMMaskXYZ);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 operator + (Vec4 a, Vec4 b)
{
	return _mm_add_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 operator - (Vec4 a, Vec4 b)
{
	return _mm_sub_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 operator * (Vec4 a, Vec4 b)
{
	return _mm_mul_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 operator / (Vec4 a, Vec4 b)
{
	return _mm_div_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 operator * (Vec4 a, float b)
{
	return _mm_mul_ps(a, _mm_set_ps(b, b, b, b));
}

//////////////////////////////////////////////////////////////////////

inline Vec4 operator / (Vec4 a, float b)
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

inline Vec3 operator - (Vec3 const &a)
{
	return Vec3(-a.x, -a.y, -a.z);
}

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

inline Vec3 Cross3(Vec3 const &a, Vec3 const &b)
{
	return Vec3(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
}

//////////////////////////////////////////////////////////////////////

