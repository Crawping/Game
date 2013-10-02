//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

void Axes::Draw(ImmediateContext *mIC, Vector origin, Vector extents, Color x, Color y, Color z)
{
	Vector vx = MaskX(extents);
	Vector vy = MaskY(extents);
	Vector vz = MaskZ(extents);

	Vector wx = Negate(vx);
	Vector wy = Negate(vy);
	Vector wz = Negate(vz);
	
	vx += origin;
	vy += origin;
	vz += origin;
	
	wx += origin;
	wy += origin;
	wz += origin;
	
	mIC->BeginLines();
	mIC->BeginVertex();	mIC->SetPosition3(vx);	mIC->SetColor(x);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(wx);	mIC->SetColor(x);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(vy);	mIC->SetColor(y);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(wy);	mIC->SetColor(y);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(vz);	mIC->SetColor(z);	mIC->EndVertex();
	mIC->BeginVertex();	mIC->SetPosition3(wz);	mIC->SetColor(z);	mIC->EndVertex();
	mIC->EndLines();
}

//////////////////////////////////////////////////////////////////////

void Axes::DrawGrid(ImmediateContext *mIC, Vector origin, Vector extents, int step, Color color)
{
	float x = GetX(extents);
	float y = GetY(extents);

	float ex = x + GetX(origin);
	float ey = y + GetY(origin);

	float ez = GetZ(origin);

	float dx = x / (float)step;
	float dy = y / (float)step;

	mIC->BeginLines();

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
	mIC->EndLines();
}
