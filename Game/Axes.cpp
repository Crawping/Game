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

void Axes::Draw(Camera &camera, Vec3 const &origin, Vec3 const &extents, Color x, Color y, Color z)
{
	Vec3 e = extents + origin;
	BeginLines(camera);
	mIC->BeginVertex();	mIC->SetPosition3(Vec3(-e.x,0,0));	mIC->SetColor(x);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec3(e.x,0,0));	mIC->SetColor(x);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec3(0,-e.y,0));	mIC->SetColor(y);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec3(0,e.y,0));	mIC->SetColor(y);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec3(0,0,-e.z));	mIC->SetColor(z);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(Vec3(0,0,e.z));	mIC->SetColor(z);	mIC->EndVertex();
	EndLines();
}

//////////////////////////////////////////////////////////////////////

void Axes::DrawGrid(Camera &camera, Vec3 const &origin, Vec3 const &extents, int step, Color color)
{
	Vec3 e = extents;
	Vec3 d = e / (float)step;
	e += origin;
	BeginLines(camera);
	for(int i=-step; i<=step; ++i)
	{
		float x1 = i * d.x;
		float y1 = i * d.y;
		float x2 = i * d.x;
		float y2 = i * d.y;
		mIC->BeginVertex();	mIC->SetPosition3(Vec3(-e.x,y1,0));	mIC->SetColor(color);	mIC->EndVertex();
		mIC->BeginVertex();	mIC->SetPosition3(Vec3( e.x,y1,0));	mIC->SetColor(color);	mIC->EndVertex();
		mIC->BeginVertex();	mIC->SetPosition3(Vec3(x1,-e.y,0));	mIC->SetColor(color);	mIC->EndVertex();
		mIC->BeginVertex();	mIC->SetPosition3(Vec3(x1, e.y,0));	mIC->SetColor(color);	mIC->EndVertex();
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

void Axes::DrawLine(Vec3 const &a, Vec3 const &b, Color ca, Color cb)
{
	mIC->BeginVertex();	mIC->SetPosition3(a);	mIC->SetColor(ca);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(b);	mIC->SetColor(cb);	mIC->EndVertex();
}

//////////////////////////////////////////////////////////////////////

void Axes::EndLines()
{
	mIC->EndLines();
}
