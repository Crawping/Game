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
static Camera *mCamera;
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

void DebugText(Vector const &pos, char const *text, ...)
{
	DebugBeginFrame();
	char buffer[1024];
	va_list v;
	va_start(v, text);
	_vsnprintf_s(buffer, ARRAYSIZE(buffer), text, v);
	Vec2 screenPos;
	DirectX::XMVECTOR vp(pos);
	DirectX::XMVECTOR sp = DirectX::XMVector3TransformCoord(vp, mCamera->GetTransformMatrix());
	float z = DirectX::XMVectorGetZ(sp);
	if(z < 1 && z > 0)
	{
		screenPos.x = (DirectX::XMVectorGetX(sp) + 1) / 2 * Graphics::FWidth();
		screenPos.y = (1 - DirectX::XMVectorGetY(sp)) / 2 * Graphics::FHeight();
		mFont->DrawString(mSpriteList, buffer, screenPos, Font::HCentre, Font::VCentre);
	}
}

//////////////////////////////////////////////////////////////////////

void DebugLine(Vector const &start, Vector const &end, Color color)
{
}

//////////////////////////////////////////////////////////////////////

