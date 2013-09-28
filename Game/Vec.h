//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class btVector3;

typedef __m128 Vec4;

//////////////////////////////////////////////////////////////////////

__declspec(align(16)) struct Vec4i
{
	union
	{
		uint32 i[4];
		Vec4 v;
	};

	operator Vec4 () const
	{
		return v;
	}
};

extern const __declspec(selectany) Vec4i gMMaskXYZ = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };

//////////////////////////////////////////////////////////////////////

#define Permute(d, c, b, a, v) _mm_shuffle_ps(v, v, ((d) << 6) | ((c) << 4) | ((b) << 2) | (a))
#define Permute2(d, c, b, a, v1, v2) _mm_shuffle_ps(v1, v2, ((d) << 6) | ((c) << 4) | ((b) << 2) | (a))

//////////////////////////////////////////////////////////////////////

Vec4		Vec(float x, float y, float z);
Vec4		Vec(float x, float y, float z, float w);

Vec4		GetX3(Vec4 a, Vec4 b, Vec4 c);
Vec4		GetY3(Vec4 a, Vec4 b, Vec4 c);
Vec4		GetZ3(Vec4 a, Vec4 b, Vec4 c);

Vec4		GetXYZ(Vec4 x, Vec4 y, Vec4 z);

Vec4		SetX(Vec4 a, float x);
Vec4		SetY(Vec4 a, float y);
Vec4		SetZ(Vec4 a, float z);
Vec4		SetW(Vec4 a, float w);

float		GetX(Vec4 v);
float		GetY(Vec4 v);
float		GetZ(Vec4 v);
float		GetW(Vec4 v);

Vec4		Negate(Vec4 v);
float		Dot(Vec4 a, Vec4 b);
float		LengthSquared(Vec4 m);
float		Length(Vec4 m);
Vec4		Normalize(Vec4 m);
Vec4		Cross(Vec4 a, Vec4 b);

Vec4		operator + (Vec4 a, Vec4 b);
Vec4		operator - (Vec4 a, Vec4 b);
Vec4		operator * (Vec4 a, Vec4 b);
Vec4		operator / (Vec4 a, Vec4 b);
Vec4		operator * (Vec4 a, float b);
Vec4		operator / (Vec4 a, float b);
Vec4 &		operator += (Vec4 &a, Vec4 b);
Vec4 &		operator -= (Vec4 &a, Vec4 b);
Vec4 &		operator *= (Vec4 &a, Vec4 b);
Vec4 &		operator *= (Vec4 &a, float b);
Vec4 &		operator /= (Vec4 &a, Vec4 b);
Vec4 &		operator /= (Vec4 &a, float b);

//////////////////////////////////////////////////////////////////////

inline Vec4 Vec(float x, float y, float z)
{
	return _mm_set_ps(0.0f, z, y, x);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 Vec(float x, float y, float z, float w)
{
	return _mm_set_ps(w, z, y, x);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 GetX3(Vec4 a, Vec4 b, Vec4 c)
{
	Vec4 t = _mm_setzero_ps();
	Vec4 xy = Permute2(0,0,0,0, a, b);
	Vec4 zw = Permute2(3,3,0,0, c, t);
	Vec4 r = Permute2(2,0,2,0, xy,zw);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 GetY3(Vec4 a, Vec4 b, Vec4 c)
{
	Vec4 t = _mm_setzero_ps();
	Vec4 xy = Permute2(1,1,1,1, a, b);
	Vec4 zw = Permute2(3,3,1,1, c, t);
	Vec4 r = Permute2(2,0,2,0, xy,zw);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 GetZ3(Vec4 a, Vec4 b, Vec4 c)
{
	Vec4 t = _mm_setzero_ps();
	Vec4 xy = Permute2(2,2,2,2, a, b);
	Vec4 zw = Permute2(3,3,2,2, c, t);
	Vec4 r = Permute2(2,0,2,0, xy,zw);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 GetXYZ(Vec4 x, Vec4 y, Vec4 z)
{
	Vec4 t = _mm_setzero_ps();
	Vec4 xy = Permute2(1,1,0,0, x, y);
	Vec4 zw = Permute2(3,3,2,2, z, t);
	Vec4 r = Permute2(2,0,2,0, xy,zw);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 SetX(Vec4 a, float x)
{
	Vec4 r = _mm_set_ss(x);
	return _mm_move_ss(a, r);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 SetY(Vec4 a, float y)
{
	Vec4 r = Permute(3,2,0,1, a);
	Vec4 t = _mm_set_ss(y);
	r = _mm_move_ss(r, t);
	return Permute(3,2,0,1, r);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 SetZ(Vec4 a, float z)
{
	Vec4 r = Permute(3,0,1,2, a);
	Vec4 t = _mm_set_ss(z);
	r = _mm_move_ss(r, t);
	return Permute(3,0,1,2, r);
}

//////////////////////////////////////////////////////////////////////

inline Vec4 SetW(Vec4 a, float w)
{
	Vec4 r = Permute(0,2,1,3, a);
	Vec4 t = _mm_set_ss(w);
	r = _mm_move_ss(r, t);
	return Permute(0,2,1,3, r);
}

//////////////////////////////////////////////////////////////////////

inline float GetX(Vec4 v)
{
	return _mm_cvtss_f32(v);
}

//////////////////////////////////////////////////////////////////////

inline float GetY(Vec4 v)
{
	return _mm_cvtss_f32(Permute(1,1,1,1, v));
}

//////////////////////////////////////////////////////////////////////

inline float GetZ(Vec4 v)
{
	return _mm_cvtss_f32(Permute(2,2,2,2, v));
}

//////////////////////////////////////////////////////////////////////

inline float GetW(Vec4 v)
{
	return _mm_cvtss_f32(Permute(3,3,3,3, v));
}

//////////////////////////////////////////////////////////////////////

inline Vec4 Negate(Vec4 v)
{
	return _mm_sub_ps(_mm_setzero_ps(), v);
}

//////////////////////////////////////////////////////////////////////

inline float Dot(Vec4 a, Vec4 b)
{
	Vec4 dot = _mm_mul_ps(a, b);
	Vec4 temp = Permute(2,1,2,1, dot);
	dot = _mm_add_ss(dot, temp);
	temp = Permute(1,1,1,1, temp);
	dot = _mm_add_ss(dot, temp);
	return GetX(dot);
}

//////////////////////////////////////////////////////////////////////

inline float LengthSquared(Vec4 m)
{
	return Dot(m, m);
}

//////////////////////////////////////////////////////////////////////

inline float Length(Vec4 m)
{
	Vec4 vLengthSq = _mm_mul_ps(m, m);
	Vec4 vTemp = Permute(2,1,2,1, vLengthSq);
	vLengthSq = _mm_add_ss(vLengthSq,vTemp);
	vTemp = Permute(1,1,1,1, vTemp);
	vLengthSq = _mm_add_ss(vLengthSq,vTemp);
	vLengthSq = Permute(0,0,0,0, vLengthSq);
	return GetX(_mm_sqrt_ps(vLengthSq));
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

// Hmmm - bullet defines conflicting functions
#if !defined (BT_USE_SSE_IN_API)

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
#endif

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

inline Vec4 &operator += (Vec4 &a, Vec4 b)
{
	a = _mm_add_ps(a, b);
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 &operator -= (Vec4 &a, Vec4 b)
{
	a = _mm_sub_ps(a, b);
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 &operator *= (Vec4 &a, Vec4 b)
{
	a = _mm_mul_ps(a, b);
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 &operator *= (Vec4 &a, float b)
{
	a = _mm_mul_ps(a, _mm_set_ps(b, b, b, b));
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 &operator /= (Vec4 &a, Vec4 b)
{
	a = _mm_div_ps(a, b);
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vec4 &operator /= (Vec4 &a, float b)
{
	a = _mm_div_ps(a, _mm_set_ps(b, b, b, b));
	return a;
}

//////////////////////////////////////////////////////////////////////

struct Vec3Floats
{
	float x;
	float y;
	float z;

	//////////////////////////////////////////////////////////////////////

	Vec3Floats()
	{
	}

	//////////////////////////////////////////////////////////////////////

	Vec3Floats(float _x, float _y, float _z)
		: x(_x)
		, y(_y)
		, z(_z)

	{
	}
};