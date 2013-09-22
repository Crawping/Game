//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

wstring GetCurrentFolder()
{
	WCHAR buffer[16384];
	buffer[0] = 0;
	GetCurrentDirectory(ARRAYSIZE(buffer), buffer);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

wstring GetExecutableFilename()
{
	WCHAR filename[32768];
	filename[0] = 0;
	filename[ARRAYSIZE(filename) - 1] = 0;
	GetModuleFileName(GetModuleHandle(null), filename, ARRAYSIZE(filename) - 1);
	return filename;
}

//////////////////////////////////////////////////////////////////////

wstring Format(WCHAR const *fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, _TRUNCATE, fmt, v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

wstring Format(wstring const &str, ...)
{
	va_list v;
	va_start(v, str);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, _TRUNCATE, str.c_str(), v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

string Format(char const *fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	char buffer[8192];
	_vsnprintf_s(buffer, _TRUNCATE, fmt, v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

string Format(string const &str, ...)
{
	va_list v;
	va_start(v, str);
	char buffer[8192];
	_vsnprintf_s(buffer, _TRUNCATE, str.c_str(), v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

void TRACE(WCHAR const *format, ...)
{
	va_list v;
	va_start(v, format);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, ARRAYSIZE(buffer) - 1, format, v);
	OutputDebugStringW(buffer);
}

//////////////////////////////////////////////////////////////////////

void TRACE(wstring const &format, ...)
{
	va_list v;
	va_start(v, format);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, ARRAYSIZE(buffer) - 1, format.c_str(), v);
	OutputDebugStringW(buffer);
}

//////////////////////////////////////////////////////////////////////

void TRACE(CHAR const *format, ...)
{
	va_list v;
	va_start(v, format);
	char buffer[8192];
	_vsnprintf_s(buffer, ARRAYSIZE(buffer) - 1, format, v);
	OutputDebugStringA(buffer);
}

//////////////////////////////////////////////////////////////////////

void TRACE(string const &format, ...)
{
	va_list v;
	va_start(v, format);
	char buffer[8192];
	_vsnprintf_s(buffer, ARRAYSIZE(buffer) - 1, format.c_str(), v);
	OutputDebugStringA(buffer);
}

//////////////////////////////////////////////////////////////////////

byte *LoadFile(TCHAR const *filename, size_t *size)
{
	byte *buf = null;
	FILE *f = null;

	if(_wfopen_s(&f, filename, TEXT("rb")) == 0)
	{
		fseek(f, 0, SEEK_END);
		uint32 len = ftell(f);
		fseek(f, 0, SEEK_SET);

		buf = new byte[len + sizeof(WCHAR)];

		if(buf != null)
		{
			size_t s = fread_s(buf, len, 1, len, f);

			if(s != len)
			{
				DeleteArray(buf);
			}
			else
			{
				*((WCHAR *)(((char *)buf) + len)) = L'\0';
				if(size != null)
				{
					*size = len;
				}
			}
		}

		fclose(f);
	}
	else
	{
		MessageBox(null, Format(TEXT("File not found: %s"), filename).c_str(), TEXT("LoadFile"), MB_ICONERROR);
	}
	return buf;
}

//////////////////////////////////////////////////////////////////////

void SaveFile(TCHAR const *filename, void *data, size_t size)
{
	FILE *f = null;
	if(_wfopen_s(&f, filename, TEXT("wb")) == 0)
	{
		size_t s = fwrite(data, sizeof(byte), size, f);
		TRACE(L"Wrote %d bytes to %s\n", s, filename);
		fclose(f);
	}
}

//////////////////////////////////////////////////////////////////////

btMatrix3x3 GetOrientedBasis(btVector3 const &z)
{
	btAssert(fabsf(z.length()-1) < 0.01f);
	btVector3 t(0,0,0);
	if(fabsf(z.z() < 0.999f))
	{
		t.setZ(1);
	}
	else
	{
		t.setX(1);
	}
	btVector3 x = t.cross(z).normalize();
	btVector3 y = z.cross(x).normalize();
	return btMatrix3x3(	x.x(), y.x(), z.x(),
						x.y(), y.y(), z.y(),
						x.z(), y.z(), z.z());
}

