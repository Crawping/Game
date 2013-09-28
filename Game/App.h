//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

ALIGN(16) class App
{
public:

	int					Run();
	void				Close();

	static bool			KeyPressed(uint keyCode);
	static bool			KeyHeld(uint keyCode);
	static bool			KeyReleased(uint keyCode);

	virtual void		OnInit() = 0;
	virtual bool		OnUpdate() = 0;
	virtual void		OnClose() = 0;

	static App &		Get();

	static int			LastKeyPressed;		// cursor keys etc
	static int			LastCharPressed;	// ascii
	static Vec2			MouseDelta;
	static float		MouseWheelDelta;
	static Vec2			MousePosition;

	static uint32		MousePressed;
	static uint32		MouseHeld;
	static uint32		MouseReleased;

	enum MouseMode
	{
		Captured,
		Free
	};

	enum MouseButton
	{
		Left = 1,
		Right = 2
	};

	static void			SetMouseMode(MouseMode mode);
	static MouseMode	GetMouseMode();

protected:

	App();
	~App();
	
	uint32 pad[3];	// +1 for the vtable

};
