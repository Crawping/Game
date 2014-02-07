//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

static App *sApp = null;

static byte kbHeld[256] = { 0 };
static byte kbPrevious[256] = { 0 };
static byte kbPressed[256] = { 0 };
static byte kbReleased[256] = { 0 };

static App::MouseMode mouseMode = App::MouseMode::Free;

int App::LastKeyPressed = 0;
int App::LastCharPressed = 0;

Vec2 App::MouseDelta(0,0);
float App::MouseWheelDelta(0);
Vec2 App::MousePosition(0,0);

uint32 App::MousePressed(0);
uint32 App::MouseHeld(0);
uint32 App::MouseReleased(0);

//////////////////////////////////////////////////////////////////////

static POINT mouseSavedPos;
static Vec2 oldMousePos;

void App::SetMouseMode(MouseMode mode)
{
	if(mode != mouseMode)
	{
		mouseMode = mode;
		if(mode == Captured)
		{
			ShowCursor(FALSE);
			GetCursorPos(&mouseSavedPos);
			oldMousePos = MousePosition;
			Graphics::UpdateMouse();
			MouseDelta = Vec2(0,0);
		}
		else
		{
			SetCursorPos(mouseSavedPos.x, mouseSavedPos.y);
			MousePosition = oldMousePos;
			ShowCursor(TRUE);
		}
	}
}

//////////////////////////////////////////////////////////////////////

void App::ResetKeyboardInput()
{
	memset(kbPressed, 0, sizeof(kbPressed));
	memset(kbHeld, 0, sizeof(kbHeld));
	memset(kbReleased, 0, sizeof(kbReleased));
	LastCharPressed = 0;
	LastKeyPressed = 0;
}

//////////////////////////////////////////////////////////////////////

App::MouseMode App::GetMouseMode()
{
	return mouseMode;
}

//////////////////////////////////////////////////////////////////////

bool App::KeyPressed(uint keyCode)
{
	return (kbPressed[keyCode] & 0x80) != 0;
}

//////////////////////////////////////////////////////////////////////

bool App::KeyHeld(uint keyCode)
{
	return (kbHeld[keyCode] & 0x80) != 0;
}

//////////////////////////////////////////////////////////////////////

bool App::KeyReleased(uint keyCode)
{
	return (kbReleased[keyCode] & 0x80) != 0;
}

//////////////////////////////////////////////////////////////////////

static void UpdateKeyboard()
{
	GetKeyboardState(kbHeld);
	for(uint i=0; i<256; ++i)
	{
		byte delta = kbHeld[i] ^ kbPrevious[i];
		kbPressed[i] = delta & kbHeld[i];
		kbReleased[i] = delta & ~kbHeld[i];
		kbPrevious[i] = kbHeld[i];
	}
}

//////////////////////////////////////////////////////////////////////

App::App()
{
	assert(sApp == null);
	sApp = this;
	TRACE("The App is at %p\n", sApp);
}

//////////////////////////////////////////////////////////////////////

App::~App()
{
}

//////////////////////////////////////////////////////////////////////

App &App::Get()
{
	return *sApp;
}

//////////////////////////////////////////////////////////////////////

void App::Close()
{
	Graphics::Close();
}

//////////////////////////////////////////////////////////////////////

int App::Run()
{
	if(Graphics::Open())
	{
		OnInit();

		while(Graphics::Update())
		{
			UpdateKeyboard();
			Joypad::Update();
			if(OnUpdate() == false)
			{
				break;
			}
		}

		OnClose();
		Close();
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////

