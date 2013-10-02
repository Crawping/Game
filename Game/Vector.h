//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

typedef __m128 Vector;

//////////////////////////////////////////////////////////////////////

ALIGN(16, struct) Vec4i
{
	union
	{
		uint32 i[4];
		Vector v;
	};

	operator Vector () const
	{
		return v;
	}
};

extern const __declspec(selectany) Vec4i gMMaskXYZ = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
extern const __declspec(selectany) Vec4i gMMaskX   = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };
extern const __declspec(selectany) Vec4i gMMaskY   = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
extern const __declspec(selectany) Vec4i gMMaskZ   = { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 };
extern const __declspec(selectany) Vec4i gMMaskW   = { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF};

//////////////////////////////////////////////////////////////////////

#pragma push_macro("Permute")
#pragma push_macro("Permute2")
#undef Permute
#undef Permute2
#define Permute(d, c, b, a, v) _mm_shuffle_ps(v, v, ((d) << 6) | ((c) << 4) | ((b) << 2) | (a))
#define Permute2(d, c, b, a, v1, v2) _mm_shuffle_ps(v1, v2, ((d) << 6) | ((c) << 4) | ((b) << 2) | (a))

//////////////////////////////////////////////////////////////////////

Vector		Vec(float x, float y, float z);
Vector		Vec(float x, float y, float z, float w);

Vector		SetX(Vector a, float x);
Vector		SetY(Vector a, float y);
Vector		SetZ(Vector a, float z);
Vector		SetW(Vector a, float w);

Vector		Splat(float x);

Vector		SplatX(Vector x);
Vector		SplatY(Vector y);
Vector		SplatZ(Vector z);
Vector		SplatW(Vector w);

Vector		MaskX(Vector x);
Vector		MaskY(Vector y);
Vector		MaskZ(Vector z);
Vector		MaskW(Vector w);

float		GetX(Vector v);
float		GetY(Vector v);
float		GetZ(Vector v);
float		GetW(Vector v);

Vector		GetX3(Vector a, Vector b, Vector c);
Vector		GetY3(Vector a, Vector b, Vector c);
Vector		GetZ3(Vector a, Vector b, Vector c);

Vector		GetXYZ(Vector x, Vector y, Vector z);

Vector		Negate(Vector v);
float		Dot(Vector a, Vector b);
float		LengthSquared(Vector v);
float		Length(Vector v);
Vector		Normalize(Vector v);
Vector		Cross(Vector a, Vector b);

Vector		operator + (Vector a, Vector b);
Vector		operator - (Vector a, Vector b);
Vector		operator * (Vector a, Vector b);
Vector		operator / (Vector a, Vector b);
Vector		operator * (Vector a, float b);
Vector		operator / (Vector a, float b);
Vector		operator * (float b, Vector a);
Vector		operator / (float b, Vector a);
Vector &	operator += (Vector &a, Vector b);
Vector &	operator -= (Vector &a, Vector b);
Vector &	operator *= (Vector &a, Vector b);
Vector &	operator *= (Vector &a, float b);
Vector &	operator /= (Vector &a, Vector b);
Vector &	operator /= (Vector &a, float b);

//////////////////////////////////////////////////////////////////////

inline Vector Vec(float x, float y, float z)
{
	return _mm_set_ps(0.0f, z, y, x);
}

//////////////////////////////////////////////////////////////////////

inline Vector Vec(float x, float y, float z, float w)
{
	return _mm_set_ps(w, z, y, x);
}

//////////////////////////////////////////////////////////////////////

inline Vector GetX3(Vector a, Vector b, Vector c)
{
	Vector t = _mm_setzero_ps();
	Vector xy = Permute2(0,0,0,0, a, b);
	Vector zw = Permute2(3,3,0,0, c, t);
	Vector r = Permute2(2,0,2,0, xy,zw);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline Vector GetY3(Vector a, Vector b, Vector c)
{
	Vector t = _mm_setzero_ps();
	Vector xy = Permute2(1,1,1,1, a, b);
	Vector zw = Permute2(3,3,1,1, c, t);
	Vector r = Permute2(2,0,2,0, xy,zw);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline Vector GetZ3(Vector a, Vector b, Vector c)
{
	Vector t = _mm_setzero_ps();
	Vector xy = Permute2(2,2,2,2, a, b);
	Vector zw = Permute2(3,3,2,2, c, t);
	Vector r = Permute2(2,0,2,0, xy,zw);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline Vector GetXYZ(Vector x, Vector y, Vector z)
{
	Vector t = _mm_setzero_ps();
	Vector xy = Permute2(1,1,0,0, x, y);
	Vector zw = Permute2(3,3,2,2, z, t);
	Vector r = Permute2(2,0,2,0, xy,zw);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline Vector SetX(Vector a, float x)
{
	Vector r = _mm_set_ss(x);
	return _mm_move_ss(a, r);
}

//////////////////////////////////////////////////////////////////////

inline Vector SetY(Vector a, float y)
{
	Vector r = Permute(3,2,0,1, a);
	Vector t = _mm_set_ss(y);
	r = _mm_move_ss(r, t);
	return Permute(3,2,0,1, r);
}

//////////////////////////////////////////////////////////////////////

inline Vector SetZ(Vector a, float z)
{
	Vector r = Permute(3,0,1,2, a);
	Vector t = _mm_set_ss(z);
	r = _mm_move_ss(r, t);
	return Permute(3,0,1,2, r);
}

//////////////////////////////////////////////////////////////////////

inline Vector SetW(Vector a, float w)
{
	Vector r = Permute(0,2,1,3, a);
	Vector t = _mm_set_ss(w);
	r = _mm_move_ss(r, t);
	return Permute(0,2,1,3, r);
}

//////////////////////////////////////////////////////////////////////

inline Vector SplatX(Vector x)
{
	return Permute(0,0,0,0, x);
}

//////////////////////////////////////////////////////////////////////

inline Vector Splat(float v)
{
	return _mm_set_ps1(v);
}

//////////////////////////////////////////////////////////////////////

inline Vector SplatY(Vector y)
{
	return Permute(1,1,1,1, y);
}

//////////////////////////////////////////////////////////////////////

inline Vector SplatZ(Vector z)
{
	return Permute(2,2,2,2, z);
}

//////////////////////////////////////////////////////////////////////

inline Vector SplatW(Vector w)
{
	return Permute(3,3,3,3, w);
}

//////////////////////////////////////////////////////////////////////

inline float GetX(Vector v)
{
	return _mm_cvtss_f32(v);
}

//////////////////////////////////////////////////////////////////////

inline float GetY(Vector v)
{
	return _mm_cvtss_f32(Permute(1,1,1,1, v));
}

//////////////////////////////////////////////////////////////////////

inline float GetZ(Vector v)
{
	return _mm_cvtss_f32(Permute(2,2,2,2, v));
}

//////////////////////////////////////////////////////////////////////

inline float GetW(Vector v)
{
	return _mm_cvtss_f32(Permute(3,3,3,3, v));
}

//////////////////////////////////////////////////////////////////////

inline Vector MaskX(Vector x)
{
	return _mm_and_ps(x, gMMaskX);
}

//////////////////////////////////////////////////////////////////////

inline Vector MaskY(Vector y)
{
	return _mm_and_ps(y, gMMaskY);
}

//////////////////////////////////////////////////////////////////////

inline Vector MaskZ(Vector z)
{
	return _mm_and_ps(z, gMMaskZ);
}

//////////////////////////////////////////////////////////////////////

inline Vector MaskW(Vector w)
{
	return _mm_and_ps(w, gMMaskW);
}

//////////////////////////////////////////////////////////////////////

inline Vector Negate(Vector v)
{
	return _mm_sub_ps(_mm_setzero_ps(), v);
}

//////////////////////////////////////////////////////////////////////

inline float Dot(Vector a, Vector b)
{
	Vector dot = _mm_mul_ps(a, b);
	Vector temp = Permute(2,1,2,1, dot);
	dot = _mm_add_ss(dot, temp);
	temp = Permute(1,1,1,1, temp);
	dot = _mm_add_ss(dot, temp);
	return GetX(dot);
}

//////////////////////////////////////////////////////////////////////

inline float LengthSquared(Vector v)
{
	return Dot(v, v);
}

//////////////////////////////////////////////////////////////////////

inline float Length(Vector v)
{
	Vector l = _mm_mul_ps(v, v);
	Vector t = Permute(2,1,2,1, l);
	l = _mm_add_ss(l,t);
	t = Permute(1,1,1,1, t);
	l = _mm_add_ss(l,t);
	return GetX(_mm_sqrt_ps(l));
}

//////////////////////////////////////////////////////////////////////

inline Vector Normalize(Vector v)
{
	Vector ls = _mm_mul_ps(v, v);
	Vector t = Permute(2,1,2,1, ls);
	ls = _mm_add_ss(ls,t);
	t = Permute(1,1,1,1, t);
	ls = _mm_add_ss(ls,t);
	ls = Permute(0,0,0,0, ls);
	return _mm_div_ps(v,_mm_sqrt_ps(ls));
}

//////////////////////////////////////////////////////////////////////

inline Vector Cross(Vector a, Vector b)
{
	Vector t1 = Permute(3,0,2,1, a);
	Vector t2 = Permute(3,1,0,2, b);
	Vector r = _mm_mul_ps(t1, t2);
	t1 = Permute(3,0,2,1, t1);
	t2 = Permute(3,1,0,2, t2);
	t1 = _mm_mul_ps(t1, t2);
	r = _mm_sub_ps(r, t1);
	return _mm_and_ps(r, gMMaskXYZ);
}

//////////////////////////////////////////////////////////////////////

// Hmmm - bullet defines conflicting functions
#if !defined (BT_USE_SSE_IN_API)

inline Vector operator + (Vector a, Vector b)
{
	return _mm_add_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline Vector operator - (Vector a, Vector b)
{
	return _mm_sub_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline Vector operator * (Vector a, Vector b)
{
	return _mm_mul_ps(a, b);
}
#endif

//////////////////////////////////////////////////////////////////////

inline Vector operator / (Vector a, Vector b)
{
	return _mm_div_ps(a, b);
}

//////////////////////////////////////////////////////////////////////

inline Vector operator * (Vector a, float b)
{
	return _mm_mul_ps(a, _mm_set_ps(b, b, b, b));
}

//////////////////////////////////////////////////////////////////////

inline Vector operator / (Vector a, float b)
{
	return _mm_div_ps(a, _mm_set_ps(b, b, b, b));
}

//////////////////////////////////////////////////////////////////////

inline Vector operator * (float b, Vector a)
{
	return _mm_mul_ps(a, _mm_set_ps(b, b, b, b));
}

//////////////////////////////////////////////////////////////////////

inline Vector operator / (float b, Vector a)
{
	return _mm_div_ps(a, _mm_set_ps(b, b, b, b));
}

//////////////////////////////////////////////////////////////////////

inline Vector &operator += (Vector &a, Vector b)
{
	a = _mm_add_ps(a, b);
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vector &operator -= (Vector &a, Vector b)
{
	a = _mm_sub_ps(a, b);
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vector &operator *= (Vector &a, Vector b)
{
	a = _mm_mul_ps(a, b);
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vector &operator *= (Vector &a, float b)
{
	a = _mm_mul_ps(a, _mm_set_ps(b, b, b, b));
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vector &operator /= (Vector &a, Vector b)
{
	a = _mm_div_ps(a, b);
	return a;
}

//////////////////////////////////////////////////////////////////////

inline Vector &operator /= (Vector &a, float b)
{
	a = _mm_div_ps(a, _mm_set_ps(b, b, b, b));
	return a;
}

//////////////////////////////////////////////////////////////////////

#pragma pop_macro("Permute")
#pragma pop_macro("Permute2")

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