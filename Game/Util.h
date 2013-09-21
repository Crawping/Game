//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

wstring GetCurrentFolder();
wstring	GetExecutableFilename();

wstring	Format(WCHAR const *fmt, ...);
wstring	Format(wstring const &fmt, ...);
string Format(char const *fmt, ...);
string Format(string const &fmt, ...);
void TRACE(WCHAR const *format, ...);
void TRACE(wstring const &format, ...);
void TRACE(char const *format, ...);
void TRACE(string const &format, ...);
byte *LoadFile(TCHAR const *filename, size_t *size);

//////////////////////////////////////////////////////////////////////

template <typename T> void Release(T * &p)
{
	if(p != null)
	{
		p->Release();
		p = null;
	}
}

//////////////////////////////////////////////////////////////////////

template <typename T> void Delete(T * &p)
{
	if(p != null)
	{
		delete p;
		p = null;
	}
}

//////////////////////////////////////////////////////////////////////

template <typename T> void DeleteArray(T * &p)
{
	if(p != null)
	{
		delete[] p;
		p = null;
	}
}

//////////////////////////////////////////////////////////////////////

inline void DX(HRESULT x)
{
	if(FAILED(x))
	{
		TRACE("DirectX Error: %08x\n", x);
		DebugBreak();
	}
}

//////////////////////////////////////////////////////////////////////
// !! BY VALUE

template <typename T> T Max(T a, T b)
{
	return a > b ? a : b;
}

//////////////////////////////////////////////////////////////////////

template <typename T> T Min(T a, T b)
{
	return a < b ? a : b;
}

//////////////////////////////////////////////////////////////////////

template <typename T> T Constrain(T a, T lower, T upper)
{
	return Min(Max(a, lower), upper);
}

//////////////////////////////////////////////////////////////////////

template <typename T> int sgn(T val)	// where T is int, short, char etc. caveat caller
{
	return (T(0) < val) - (val < T(0)); 
}

//////////////////////////////////////////////////////////////////////
template <typename T> void Swap(T &a, T &b)
{
	T t = a;
	a = b;
	b = t;
}

//////////////////////////////////////////////////////////////////////

inline float Ease(float d)
{
	float d2 = d * d;
	float d3 = d2 * d;
	return 3 * d2 - 2 * d3;
}

//////////////////////////////////////////////////////////////////////

inline string ToLower(string const &s)
{
	string r(s);
	std::transform(r.begin(), r.end(), r.begin(), ::tolower);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline wstring ToLower(wstring const &s)
{
	wstring r(s);
	std::transform(r.begin(), r.end(), r.begin(), ::tolower);
	return r;
}

//////////////////////////////////////////////////////////////////////

inline int NextPowerOf2(int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

//////////////////////////////////////////////////////////////////////

btMatrix3x3 GetOrientedBasis(btVector3 const &z);
