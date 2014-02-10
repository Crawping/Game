//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

static bool opened = false;
static bool inframe = false;
static VertexShader *mVS = null;
static PixelShader *mPS = null;
static Material *mMaterial = null;
static ImmediateContext *mIC = null;
static Font *mFont = null;
static SpriteList *mSpriteList = null;
static Camera *mCamera = null;
static Camera *mOldCamera = null;
static Vec2 mCursorPos;

//////////////////////////////////////////////////////////////////////

void DebugOpen()
{
	uint32 mask = VertexElement::Position3 + VertexElement::Color;
	mVS = VertexShader::Load(L"UntexturedVertexShader", mask);
	mPS = PixelShader::Load(L"UntexturedPixelShader");
	mMaterial = Material::Create(mPS, mVS, BM_Modulate);
	mIC = ImmediateContext::Create(mask, 65536, 65536);
	mFont = Font::Load(L"debug");
	mSpriteList = SpriteList::Create();
	opened = true;
}

//////////////////////////////////////////////////////////////////////

void DebugClose()
{
	DebugEndFrame();
	Release(mIC);
	Release(mMaterial);
	Release(mPS);
	Release(mVS);
	Release(mFont);
	Release(mSpriteList);
}

//////////////////////////////////////////////////////////////////////

void DebugSetCamera(Camera *camera)
{
	mCamera = camera;
	if(mCamera != mOldCamera && inframe)
	{
		mIC->SetConstants(&mCamera->GetTransformMatrix(), sizeof(Matrix));
	}
	mOldCamera = mCamera;
}

//////////////////////////////////////////////////////////////////////

void DebugBeginFrame()
{
	if(!opened)
	{
		DebugOpen();
	}
	if(!inframe)
	{
		mCursorPos.Set(0,0);
		inframe = true;
		mIC->Begin();
		mIC->SetMaterial(mMaterial);
		mIC->SetConstants(&mCamera->GetTransformMatrix(), sizeof(Matrix));	// possibly redundant
		mSpriteList->Begin();
		mSpriteList->SetZBufferMode(ZB_Disable);
		mSpriteList->SetBlendMode(BM_Modulate);
		mSpriteList->SetViewport(0, 0, Graphics::Width(), Graphics::Height());
	}
}

//////////////////////////////////////////////////////////////////////

void DebugEndFrame()
{
	if(inframe)
	{
		inframe = false;
		mIC->End();
		mSpriteList->End();
	}
}

//////////////////////////////////////////////////////////////////////

void DebugText(char const *text, ...)
{
	char buffer[1024];
	va_list v;
	va_start(v, text);
	size_t len = _vsnprintf_s(buffer, ARRAYSIZE(buffer), text, v);
	DebugBeginFrame();
	mFont->DrawString(mSpriteList, buffer, mCursorPos);
}

//////////////////////////////////////////////////////////////////////

void DebugText(Vec2 const &pos, char const *text, ...)
{
	char buffer[1024];
	va_list v;
	va_start(v, text);
	_vsnprintf_s(buffer, ARRAYSIZE(buffer), text, v);
	DebugBeginFrame();
	Vec2 p(pos);
	mFont->DrawString(mSpriteList, buffer, p);
}

//////////////////////////////////////////////////////////////////////

void DebugText(Point2D const &pos, char const *text, ...)
{
	char buffer[1024];
	va_list v;
	va_start(v, text);
	_vsnprintf_s(buffer, ARRAYSIZE(buffer), text, v);
	DebugBeginFrame();
	Vec2 p(pos);
	mFont->DrawString(mSpriteList, buffer, p);
}

//////////////////////////////////////////////////////////////////////

void DebugText(int x, int y, char const *text, ...)
{
	char buffer[1024];
	va_list v;
	va_start(v, text);
	_vsnprintf_s(buffer, ARRAYSIZE(buffer), text, v);
	DebugBeginFrame();
	Vec2 p(x, y);
	mFont->DrawString(mSpriteList, buffer, p);
}

//////////////////////////////////////////////////////////////////////

void DebugText(Vec4f const &pos, char const *text, ...)
{
	DebugBeginFrame();
	char buffer[1024];
	va_list v;
	va_start(v, text);
	_vsnprintf_s(buffer, ARRAYSIZE(buffer), text, v);

	Vec4f r = TransformPoint(pos, mCamera->GetTransformMatrix());
	float z = GetZ(r);
	if(z < 1 && z > 0)
	{
		float x = floorf(0.5f + ((GetX(r) + 1) / 2 * Graphics::FWidth()));
		float y = floorf(0.5f + ((1 - GetY(r)) / 2 * Graphics::FHeight()));
		Vec2 pos(x, y);
		mFont->DrawString(mSpriteList, buffer, pos, Font::HCentre, Font::VCentre);
	}
}

//////////////////////////////////////////////////////////////////////

static void _DrawCircle(Matrix const &transform, int xaxis, int yaxis, int zaxis, float z, float radius, int segments, Color color)
{
	float p[3];
	float s8p = segments / TWO_PI;
	float t0 = 0;
	for(int t=0; t<segments; ++t)
	{
		float t1 = (t + 1) / s8p;
		float x0 = sinf(t0) * radius;
		float y0 = cosf(t0) * radius;
		float x1 = sinf(t1) * radius;
		float y1 = cosf(t1) * radius;
		p[xaxis] = x0;
		p[yaxis] = y0;
		p[zaxis] = z;
		Vec4f s = Vec4(p[0], p[1], p[2]);
		p[xaxis] = x1;
		p[yaxis] = y1;
		Vec4f e = Vec4(p[0], p[1], p[2]);
		DebugLine(TransformPoint(s, transform), TransformPoint(e, transform), color);
		t0 = t1;
	}
}

//////////////////////////////////////////////////////////////////////

void DebugSphere(Matrix const &transform, float radius, Color color, int segments, int smoothness)
{
	DebugBeginLines();

	float rsquared = radius * radius;
	for(int i=-segments; i<segments; ++i)
	{
		float n = (radius * i) / segments;
		float r = sqrtf(rsquared - n * n);
		float z = i / (segments / radius);
		_DrawCircle(transform, 0, 1, 2, z, r, smoothness, color);
		_DrawCircle(transform, 1, 2, 0, z, r, smoothness, color);
		_DrawCircle(transform, 2, 0, 1, z, r, smoothness, color);
	}
	DebugEndLines();
}

//////////////////////////////////////////////////////////////////////

void DebugCylinder(Matrix const &transform, float radius, float width, Color color, int segments /* = 2 */, int smoothness /* = 16 */)
{
	DebugBeginLines();
	_DrawCircle(transform, 0, 2, 1, -width, radius, smoothness, color);
	_DrawCircle(transform, 0, 2, 1, +width, radius, smoothness, color);
	float tscale = segments / TWO_PI;
	for(int i=0; i<segments; ++i)
	{
		float t = i / tscale;
		float x0 = sinf(t) * radius;
		float z0 = cosf(t) * radius;
		float y0 = -width;
		float y1 = +width;
		DebugLine(TransformPoint(Vec4(x0, y0, z0), transform), TransformPoint(Vec4(x0, y1, z0), transform), color);
	}
	DebugEndLines();
}

//////////////////////////////////////////////////////////////////////

void DebugBeginLines()
{
	DebugBeginFrame();
	mIC->BeginLines();
}

//////////////////////////////////////////////////////////////////////

void DebugEndLines()
{
	mIC->EndLines();
}

//////////////////////////////////////////////////////////////////////

void DebugLine(Vec4f const &start, Vec4f const &end, Color color)
{
	mIC->BeginVertex();
	mIC->SetPosition3(start);
	mIC->SetColor(color);
	mIC->EndVertex();
	mIC->BeginVertex();
	mIC->SetPosition3(end);
	mIC->SetColor(color);
	mIC->EndVertex();
}

//////////////////////////////////////////////////////////////////////

void DebugOneLine(Vec4f const &start, Vec4f const &end, Color color)
{
	DebugBeginLines();
	DebugLine(start, end, color);
	DebugEndLines();
}

//////////////////////////////////////////////////////////////////////

void DebugAxes(Vec4f const &pos, float length)
{
	DebugBeginLines();
	Vec4f x = Vec4(length, 0, 0);
	Vec4f y = Vec4(0, length, 0);
	Vec4f z = Vec4(0, 0, length);
	DebugLine(pos - x, pos + x, 0xff0000ff);
	DebugLine(pos - y, pos + y, 0x00ff00ff);
	DebugLine(pos - z, pos + z, 0x0000ffff);
	DebugEndLines();
}
