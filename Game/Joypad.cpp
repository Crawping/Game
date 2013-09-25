//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <xinput.h>

//////////////////////////////////////////////////////////////////////

namespace
{
	struct Pad
	{
		XINPUT_STATE	mState;
		bool			mConnected;
		uint32			mHeld;
		uint32			mPressed;
		uint32			mReleased;
		Vec2			mStick[2];
		float			mTrigger[2];
	};

	Pad pad[4] = { 0 };
}

//////////////////////////////////////////////////////////////////////

namespace Joypad
{
	//////////////////////////////////////////////////////////////////////

	void Open()
	{
	}

	//////////////////////////////////////////////////////////////////////

	void Update()
	{
		for(uint i=0; i<4; ++i)
		{
			Pad &p = pad[i];
			if(XInputGetState(i, &p.mState) == ERROR_SUCCESS)
			{
				p.mConnected = true;
				XINPUT_GAMEPAD &joypad = p.mState.Gamepad;
				uint32 old = p.mHeld;
				p.mHeld = joypad.wButtons;
				uint32 delta = p.mHeld ^ old;
				p.mPressed = delta & p.mHeld;
				p.mReleased = delta & ~p.mHeld;
				p.mTrigger[0] = joypad.bLeftTrigger / 255.0f;
				p.mTrigger[1] = joypad.bRightTrigger / 255.0f;
				p.mStick[0] = Vec2(joypad.sThumbLX / 32768.0f, (float)joypad.sThumbLY / 32768.0f);
				p.mStick[1] = Vec2(joypad.sThumbRX / 32768.0f, (float)joypad.sThumbRY / 32768.0f);
			}
			else
			{
				p.mConnected = false;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////

	void Close()
	{
	}

	//////////////////////////////////////////////////////////////////////

	bool IsConnected(uint index)
	{
		return pad[index].mConnected;
	}

	//////////////////////////////////////////////////////////////////////

	uint32 Held(uint index, uint32 buttons)
	{
		return pad[index].mHeld & buttons;
	}

	//////////////////////////////////////////////////////////////////////

	uint32 Pressed(uint index, uint32 buttons)
	{
		return pad[index].mPressed & buttons;
	}

	//////////////////////////////////////////////////////////////////////

	uint32 Released(uint index, uint32 buttons)
	{
		return pad[index].mReleased & buttons;
	}

	//////////////////////////////////////////////////////////////////////

	Vec2 Stick(uint index, int stickIndex)
	{
		return pad[index].mStick[stickIndex];
	}
	
	//////////////////////////////////////////////////////////////////////

	float Trigger(uint index, int triggerIndex)
	{
		return pad[index].mTrigger[triggerIndex];
	}
}
