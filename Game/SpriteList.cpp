//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

SpriteList::SpriteList()
{
	mProjection = Camera::OrthoProjection2D(Graphics::Width(), Graphics::Height());
	uint32 mask = VertexElement::Position2 | VertexElement::TexCoord | VertexElement::Color;
	mIC = ImmediateContext::Create(mask, 65536, sizeof(Matrix));
	mPixelShader = PixelShader::Load(L"2DPixelShader");
	mVertexShader = VertexShader::Load(L"2DVertexShader", mask);
	mMaterial = Material::Create(mPixelShader, mVertexShader, BM_Additive);
}

//////////////////////////////////////////////////////////////////////

SpriteList::~SpriteList()
{
	::Release(mPixelShader);
	::Release(mVertexShader);
	::Release(mIC);
	::Release(mMaterial);
}

//////////////////////////////////////////////////////////////////////

SpriteList *SpriteList::Create()
{
	return new SpriteList();
}

//////////////////////////////////////////////////////////////////////

void SpriteList::Begin()
{
	mIC->Begin();
	mIC->SetMaterial(mMaterial);
	mIC->SetConstants(&mProjection, sizeof(Matrix));
}

//////////////////////////////////////////////////////////////////////

void SpriteList::SetViewport(int left, int top, int right, int bottom)
{
	mIC->SetViewport(left, top, right, bottom);
}

//////////////////////////////////////////////////////////////////////

void SpriteList::SetTexture(Texture const *texture)
{
	mIC->SetTexture(texture);
	mTextureWidth = (float)texture->Width();
	mTextureHeight = (float)texture->Height();
}

//////////////////////////////////////////////////////////////////////

void SpriteList::SetBlendMode(BlendMode blendMode)
{
	mIC->SetBlendMode(blendMode);
}

//////////////////////////////////////////////////////////////////////

void SpriteList::SetZBufferMode(ZBufferMode mode)
{
	mIC->SetZBufferMode(mode);
}

//////////////////////////////////////////////////////////////////////

void SpriteList::AddSprite(Vec2 const &topLeft, Vec2 const &size, Vec2 const &uvTopLeft, Vec2 const &uvBottomRight, Color color)
{
	Vec2 const &tl = topLeft;
	Vec2 br = topLeft + size;
	Vec2 tr(br.x, topLeft.y);
	Vec2 bl(topLeft.x, br.y);

	Vec2 const &utl = uvTopLeft;
	Vec2 const &ubr = uvBottomRight;
	Vec2 utr(uvBottomRight.x, uvTopLeft.y);
	Vec2 ubl(uvTopLeft.x, uvBottomRight.y);

	mIC->BeginTriangles();

	mIC->BeginVertex();	mIC->SetPosition2(tl);	mIC->SetTexCoord(utl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(tr);	mIC->SetTexCoord(utr);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(bl);	mIC->SetTexCoord(ubl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(bl);	mIC->SetTexCoord(ubl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(tr);	mIC->SetTexCoord(utr);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(br);	mIC->SetTexCoord(ubr);	mIC->SetColor(color);	mIC->EndVertex();

	mIC->EndTriangles();
}

//////////////////////////////////////////////////////////////////////

void SpriteList::BeginSpriteRun()
{
	mIC->BeginTriangles();
}

//////////////////////////////////////////////////////////////////////

void SpriteList::EndSpriteRun()
{
	mIC->EndTriangles();
}

//////////////////////////////////////////////////////////////////////

void SpriteList::AddSpriteEx(Vec2 const &topLeft, Vec2 const &size, Vec2 const &uvTopLeft, Vec2 const &uvBottomRight, Color color)
{
	Vec2 const &tl = topLeft;
	Vec2 br = topLeft + size;
	Vec2 tr(br.x, topLeft.y);
	Vec2 bl(topLeft.x, br.y);

	Vec2 const &utl = uvTopLeft;
	Vec2 const &ubr = uvBottomRight;
	Vec2 utr(uvBottomRight.x, uvTopLeft.y);
	Vec2 ubl(uvTopLeft.x, uvBottomRight.y);

	mIC->BeginVertex();	mIC->SetPosition2(tl);	mIC->SetTexCoord(utl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(tr);	mIC->SetTexCoord(utr);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(bl);	mIC->SetTexCoord(ubl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(bl);	mIC->SetTexCoord(ubl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(tr);	mIC->SetTexCoord(utr);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(br);	mIC->SetTexCoord(ubr);	mIC->SetColor(color);	mIC->EndVertex();
}

//////////////////////////////////////////////////////////////////////

void SpriteList::AddSprite(Vec2 const &topLeft, Vec2 const &size, Point2D const &uvTopLeft, Point2D const &uvBottomRight, Color color)
{
	Vec2 const &tl = topLeft;
	Vec2 br = topLeft + size;
	Vec2 tr(br.x, topLeft.y);
	Vec2 bl(topLeft.x, br.y);

	Vec2 utl = Vec2(uvTopLeft.x / mTextureWidth, uvTopLeft.y / mTextureHeight);
	Vec2 ubr = Vec2(uvBottomRight.x / mTextureWidth, uvBottomRight.y / mTextureHeight);
	Vec2 utr(uvBottomRight.x / mTextureWidth, uvTopLeft.y / mTextureHeight);
	Vec2 ubl(uvTopLeft.x / mTextureWidth, uvBottomRight.y /mTextureHeight);

	mIC->BeginTriangles();

	mIC->BeginVertex();	mIC->SetPosition2(tl);	mIC->SetTexCoord(utl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(tr);	mIC->SetTexCoord(utr);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(bl);	mIC->SetTexCoord(ubl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(bl);	mIC->SetTexCoord(ubl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(tr);	mIC->SetTexCoord(utr);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(br);	mIC->SetTexCoord(ubr);	mIC->SetColor(color);	mIC->EndVertex();

	mIC->EndTriangles();
}

//////////////////////////////////////////////////////////////////////

void SpriteList::AddSpriteEx(Vec2 const &topLeft, Vec2 const &size, Point2D const &uvTopLeft, Point2D const &uvBottomRight, Color color)
{
	Vec2 const &tl = topLeft;
	Vec2 br = topLeft + size;
	Vec2 tr(br.x, topLeft.y);
	Vec2 bl(topLeft.x, br.y);

	Vec2 utl = Vec2(uvTopLeft.x / mTextureWidth, uvTopLeft.y / mTextureHeight);
	Vec2 ubr = Vec2(uvBottomRight.x / mTextureWidth, uvBottomRight.y / mTextureHeight);
	Vec2 utr(uvBottomRight.x / mTextureWidth, uvTopLeft.y / mTextureHeight);
	Vec2 ubl(uvTopLeft.x / mTextureWidth, uvBottomRight.y /mTextureHeight);

	mIC->BeginVertex();	mIC->SetPosition2(tl);	mIC->SetTexCoord(utl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(tr);	mIC->SetTexCoord(utr);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(bl);	mIC->SetTexCoord(ubl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(bl);	mIC->SetTexCoord(ubl);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(tr);	mIC->SetTexCoord(utr);	mIC->SetColor(color);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition2(br);	mIC->SetTexCoord(ubr);	mIC->SetColor(color);	mIC->EndVertex();
}

//////////////////////////////////////////////////////////////////////

void SpriteList::End()
{
	mIC->End();
}
