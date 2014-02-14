//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

void Axes::Draw(ImmediateContext *mIC, Vec4f origin, Vec4f extents, Color x, Color y, Color z)
{
	Vec4f vx = MaskX(extents);
	Vec4f vy = MaskY(extents);
	Vec4f vz = MaskZ(extents);

	Vec4f wx = Negate(vx) + origin;
	Vec4f wy = Negate(vy) + origin;
	Vec4f wz = Negate(vz) + origin;
	
	vx += origin;
	vy += origin;
	vz += origin;
	
	mIC->BeginVertex();	mIC->SetPosition3(vx);	mIC->SetColor(x);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(wx);	mIC->SetColor(x);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(vy);	mIC->SetColor(y);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(wy);	mIC->SetColor(y);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(vz);	mIC->SetColor(z);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(wz);	mIC->SetColor(z);	mIC->EndVertex();
}

//////////////////////////////////////////////////////////////////////

void Axes::DrawGrid(ImmediateContext *mIC, Vec4f origin, Vec4f extents, int step, Color color)
{
	float x = GetX(extents);
	float y = GetY(extents);

	float ex = x + GetX(origin);
	float ey = y + GetY(origin);

	float ez = GetZ(origin);

	float dx = x / (float)step;
	float dy = y / (float)step;

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
}
