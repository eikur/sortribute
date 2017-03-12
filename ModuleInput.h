#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Point.h"

#include "3rdparty/SDL/include/SDL_scancode.h"

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum GamepadButton
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
	START = 4,
	SELECT = 5,
	L3 = 6,
	R3 = 7,
	LB = 8,
	RB = 9,
	A = 10,
	B = 11,
	X = 12,
	Y
};

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	bool UsingGamepad() const {
		return gamepad != nullptr;
	}

	KeyState GetGamepadButton(int id) const
	{
		if (gamepad == nullptr)
			return KEY_IDLE;
		else
			return gamepad_buttons[id];
	}

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;

	// Get mouse / axis position
	const iPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;

	// Load configuration from file
	bool LoadConfigFromFile();

private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	iPoint mouse_motion;
	iPoint mouse;

	SDL_Joystick *gamepad = nullptr;
	KeyState*	gamepad_buttons;
	int num_gamepad_buttons;

	int m_screen_size = 0;

};

#endif // __MODULEINPUT_H__