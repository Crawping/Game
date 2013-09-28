//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

Axes::Axes()
{
	uint32 mask = VertexElement::Position3 + VertexElement::Color;
	mVS = VertexShader::Load(L"UntexturedVertexShader", mask);
	mPS = PixelShader::Load(L"UntexturedPixelShader");
	mMaterial = Material::Create(mPS, mVS, BM_Modulate);
	mIC = ImmediateContext::Create(mask, 65536, 65536);
}

//////////////////////////////////////////////////////////////////////

Axes::~Axes()
{
	Release(mVS);
	Release(mPS);
	Release(mMaterial);
	Release(mIC);
}

//////////////////////////////////////////////////////////////////////

void Axes::Begin()
{
	mIC->Begin();
}

//////////////////////////////////////////////////////////////////////

void Axes::End()
{
	mIC->End();
}

//////////////////////////////////////////////////////////////////////

void Axes::Draw(Camera &camera, Vector origin, Vector extents, Color x, Color y, Color z)
{
	Vector e = extents + origin;
	float ex = GetX(e);
	float ey = GetY(e);
	float ez = GetZ(e);
	BeginLines(camera);
	mIC->BeginVertex();	mIC->SetPosition3(Vec(-ex,0,0));	mIC->SetColor(x);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec(ex,0,0));		mIC->SetColor(x);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec(0,-ey,0));	mIC->SetColor(y);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec(0,ey,0));		mIC->SetColor(y);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec(0,0,-ez));	mIC->SetColor(z);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec(0,0,ez));		mIC->SetColor(z);	mIC->EndVertex();
	EndLines();
}

//////////////////////////////////////////////////////////////////////

void Axes::DrawGrid(Camera &camera, Vector origin, Vector extents, int step, Color color)
{
	float x = GetX(extents);
	float y = GetY(extents);
	float ex = x + GetX(origin);
	float ey = y + GetY(origin);
	float ez = GetZ(origin);
	float dx = x / (float)step;
	float dy = y / (float)step;
	BeginLines(camera);
	for(int i=-step; i<=step; ++i)
	{
		float x1 = i * dx;
		float y1 = i * dy;
		float x2 = i * dx;
		float y2 = i * dy;
		mIC->BeginVertex();	mIC->SetPosition3(-ex,y1,ez);	mIC->SetColor(color);	mIC->EndVertex();
		mIC->BeginVertex();	mIC->SetPosition3( ex,y1,ez);	mIC->SetColor(color);	mIC->EndVertex();
		mIC->BeginVertex();	mIC->SetPosition3(x1,-ey,ez);	mIC->SetColor(color);	mIC->EndVertex();
		mIC->BeginVertex();	mIC->SetPosition3(x1, ey,ez);	mIC->SetColor(color);	mIC->EndVertex();
	}
	EndLines();
}

//////////////////////////////////////////////////////////////////////

void Axes::BeginLines(Camera &camera)
{
	mIC->SetMaterial(mMaterial);
	mIC->SetConstants(&camera.GetTransformMatrix(), sizeof(Matrix));
	//mIC->SetBlendMode(BM_Modulate);
	mIC->BeginLines();
}

//////////////////////////////////////////////////////////////////////

void Axes::DrawLine(Vector a, Vector b, Color ca, Color cb)
{
	mIC->BeginVertex();	mIC->SetPosition3(a);	mIC->SetColor(ca);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(b);	mIC->SetColor(cb);	mIC->EndVertex();
}

//////////////////////////////////////////////////////////////////////

void Axes::EndLines()
{
	mIC->EndLines();
}
