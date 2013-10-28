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
	
	Matrix const &m = mCamera->GetTransformMatrix();

	Vec4f r = SplatX(pos) * m.r[0] + SplatY(pos) * m.r[1] + SplatZ(pos) * m.r[2] + m.r[3];
	r /= SplatW(r);

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

static void _DrawCircle(Vec4f pos, int xaxis, int yaxis, int zaxis, float z, float radius, int segments, Color color)
{
	float p[3];
	float s8p = segments / TWO_PI;

	float t0 = 0;

//	float z = i / (segments / radius);

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
		Vec4f s = Vec4(p[0], p[1], p[2]) + pos;
		p[xaxis] = x1;
		p[yaxis] = y1;
		Vec4f e = Vec4(p[0], p[1], p[2]) + pos;
		DebugLine(s, e, color);
		t0 = t1;
	}
}

//////////////////////////////////////////////////////////////////////

void DebugSphere(Vec4f const &pos, float radius, Color color, int segments)
{
	DebugBeginLines();

	float rsquared = radius * radius;

	segments /= 2;

	for(int i=-segments; i<segments; ++i)
	{
		float n = (radius * i) / segments;
		float r = sqrtf(rsquared - n * n);

		int s8 = segments * 8;
		float s8p = s8 / TWO_PI;

		float t0 = 0;

		float z = i / (segments / radius);
		_DrawCircle(pos, 0, 1, 2, z, r, s8, color);
		_DrawCircle(pos, 1, 2, 0, z, r, s8, color);
		_DrawCircle(pos, 2, 0, 1, z, r, s8, color);
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

