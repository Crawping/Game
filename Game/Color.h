//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

typedef uint32 Color;

//////////////////////////////////////////////////////////////////////

inline Color FromRGBA(int r, int g, int b, int a)
{
	return (a & 0xff) | ((b & 0xff) << 8) | ((g & 0xff) << 16) | ((r & 0xff) << 24);
}

//////////////////////////////////////////////////////////////////////

inline Color FromRGBAf(float r, float g, float b, float a)
{
	return FromRGBA((int)(a * 255.0f), (int)(b * 255.0f), (int)(g * 255.0f), (int)(r * 255.0f));
}

//////////////////////////////////////////////////////////////////////

inline Color FromRGB(int r, int g, int b)
{
	return FromRGBA(r, g, b, 0xff);
}

//////////////////////////////////////////////////////////////////////

inline Color FromRGBf(float r, float g, float b)
{
	return FromRGBAf(r, g, b, 1);
}

//////////////////////////////////////////////////////////////////////

inline int GetRed(Color c)
{
	return (c >>  24) & 0xff;
}

//////////////////////////////////////////////////////////////////////

inline int GetGreen(Color c)
{
	return (c >>  16) & 0xff;
}

//////////////////////////////////////////////////////////////////////

inline int GetBlue(Color c)
{
	return (c >> 8) & 0xff;
}

//////////////////////////////////////////////////////////////////////

inline int GetAlpha(Color c)
{
	return (c >> 0) & 0xff;
}

//////////////////////////////////////////////////////////////////////

inline Color ColorFromVec4f(Vec4f v)
{
	v *= 255.0f;
	return FromRGBA((int)GetW(v), (int)GetZ(v), (int)GetY(v), (int)GetX(v));
}

//////////////////////////////////////////////////////////////////////

inline Vec4f Vec4FromColor_RGBA(Color c)
{
	return Vec4((float)GetRed(c), (float)GetGreen(c), (float)GetBlue(c), (float)GetAlpha(c)) / 255.0f;;
}

//////////////////////////////////////////////////////////////////////

inline Vec4f Vec4FromColor_ABGR(Color c)
{
	return Vec4((float)GetAlpha(c), (float)GetBlue(c), (float)GetGreen(c), (float)GetRed(c)) / 255.0f;;
}

//////////////////////////////////////////////////////////////////////

inline void SetRGBAFloatsFromColor(float *f, Color c)
{
	f[0] = GetRed(c) / 255.0f;
	f[1] = GetGreen(c) / 255.0f;
	f[2] = GetBlue(c) / 255.0f;
	f[3] = GetAlpha(c) / 255.0f;
}

//////////////////////////////////////////////////////////////////////

inline void SetABGRFloatsFromColor(float *f, Color c)
{
	f[0] = GetAlpha(c) / 255.0f;
	f[1] = GetBlue(c) / 255.0f;
	f[2] = GetGreen(c) / 255.0f;
	f[3] = GetRed(c) / 255.0f;
}

//////////////////////////////////////////////////////////////////////

inline Color GetColorFromRGBAFloats(float *f)
{
	return FromRGBA((int)(f[0] * 255.0f),
					(int)(f[1] * 255.0f),
					(int)(f[2] * 255.0f),
					(int)(f[3] * 255.0f));
}
