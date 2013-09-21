//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

namespace Joypad
{
	enum
	{
		Up				= XINPUT_GAMEPAD_DPAD_UP,
		Down			= XINPUT_GAMEPAD_DPAD_DOWN,
		Left			= XINPUT_GAMEPAD_DPAD_LEFT,
		Right			= XINPUT_GAMEPAD_DPAD_RIGHT,
		Start			= XINPUT_GAMEPAD_START,
		Back			= XINPUT_GAMEPAD_BACK,
		LeftThumb		= XINPUT_GAMEPAD_LEFT_THUMB,
		RightThumb		= XINPUT_GAMEPAD_RIGHT_THUMB,
		LeftShoulder	= XINPUT_GAMEPAD_LEFT_SHOULDER,
		RightShoulder	= XINPUT_GAMEPAD_RIGHT_SHOULDER,
		A				= XINPUT_GAMEPAD_A,
		B				= XINPUT_GAMEPAD_B,
		X				= XINPUT_GAMEPAD_X,
		Y				= XINPUT_GAMEPAD_Y,
		All				= 0xffff
	};

	bool IsConnected(uint index);
	void Update();

	uint32 Held(uint index, uint32 buttons = All);
	uint32 Pressed(uint index, uint32 buttons = All);
	uint32 Released(uint index, uint32 buttons = All);
	Vec2 Stick(uint index, int stickIndex);
	float Trigger(uint index, int triggerIndex);

}