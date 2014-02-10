//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

static POINT mouseSavedPos;
static Vec2 oldMousePos;
static Mouse::Mode mouseMode = Mouse::Mode::Free;

//////////////////////////////////////////////////////////////////////

namespace Mouse
{
	//////////////////////////////////////////////////////////////////////

	Vec2		Delta(0,0);
	float		WheelDelta(0);
	Vec2		Position(0,0);
	uint32		Pressed(0);
	uint32		Held(0);
	uint32		Released(0);

	//////////////////////////////////////////////////////////////////////

	void Update()
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(Graphics::hwnd, &p);
		Position = Vec2((float)p.x, (float)p.y);
		if(GetMode() == Mode::Captured)
		{
			POINT c;
			c.x	= Graphics::Width() / 2;
			c.y	= Graphics::Height() / 2;
			Mouse::Delta = Vec2(p.x - c.x, p.y - c.y);
			ClientToScreen(Graphics::hwnd, &c);
			SetCursorPos(c.x, c.y);
		}
	}

	//////////////////////////////////////////////////////////////////////

	void SetMode(Mode mode)
	{
		if(mouseMode != mode)
		{
			mouseMode = mode;
			if(mode == Captured)
			{
				ShowCursor(FALSE);
				GetCursorPos(&mouseSavedPos);
				oldMousePos = Mouse::Position;
				Mouse::Update();
				Mouse::Delta = Vec2(0,0);
			}
			else
			{
				SetCursorPos(mouseSavedPos.x, mouseSavedPos.y);
				Mouse::Position = oldMousePos;
				ShowCursor(TRUE);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////

	Mode GetMode()
	{
		return mouseMode;
	}

	//////////////////////////////////////////////////////////////////////

}
