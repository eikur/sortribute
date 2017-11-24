#include "Globals.h"
#include "Application.h"
#include "ConfigurationLoader.h"

#include "ModuleInput.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module(), mouse({0, 0}), mouse_motion({0,0})
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(keyboard);
	if (gamepad != nullptr)
		RELEASE_ARRAY(gamepad_buttons);
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	SDL_Init(0);
	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
	{
		LOG("SDL_JOYSTICK could not be initialized! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		if (SDL_NumJoysticks() <= 0)
		{
			LOG("Input: No gamepad detected");
		}
		else
		{
			LOG("Input: Gamepad detected");
			SDL_JoystickEventState(SDL_ENABLE);
			gamepad = SDL_JoystickOpen(0);
			if (gamepad != nullptr)
			{
				num_gamepad_buttons = SDL_JoystickNumButtons(gamepad);
				gamepad_buttons = new KeyState[num_gamepad_buttons];
				memset(gamepad_buttons, KEY_IDLE, sizeof(KeyState) * num_gamepad_buttons);
			}
		}

	}
	if(SDL_InitSubSystem(SDL_INIT_EVENTS ) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	if (LoadConfigFromFile() == false)
	{
		LOG("Input: Unable to load from config file\n");
		return false;
	}


	return true;
}

// Called before the first frame
bool ModuleInput::Start()
{
	return true;
}

// Called each loop iteration
UpdateStatus ModuleInput::PreUpdate()
{
	static SDL_Event event;

	mouse_motion = { 0, 0 };
	memset(windowEvents, false, WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}
	if (gamepad != nullptr)
	{
		for (int i = 0; i < num_gamepad_buttons; i++)
		{
			if (gamepad_buttons[i] == KEY_DOWN)
				gamepad_buttons[i] = KEY_REPEAT;

			if (gamepad_buttons[i] == KEY_UP)
				gamepad_buttons[i] = KEY_IDLE;

		}
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

			case SDL_MOUSEMOTION:
				mouse_motion.x = event.motion.xrel / m_screen_size;	
				mouse_motion.y = event.motion.yrel / m_screen_size;	
				mouse.x = event.motion.x / m_screen_size;			
				mouse.y = event.motion.y / m_screen_size;			
			break;
			case SDL_JOYBUTTONDOWN:
				gamepad_buttons[event.jbutton.button] = KEY_DOWN;
			break;
			case SDL_JOYBUTTONUP:
				gamepad_buttons[event.jbutton.button] = KEY_UP;
			break;
		}
	}

	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		return UpdateStatus::Stop;
	}

	return UpdateStatus::Continue;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	//SDL_JoystickClose(gamepad);
	SDL_QuitSubSystem(SDL_INIT_EVENTS | SDL_INIT_JOYSTICK);
	
	return true;
}

// ---------
bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}

const iPoint& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const iPoint& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}

bool ModuleInput::LoadConfigFromFile()
{
	JSON_Object *window_object = App->getConfig().GetJSONObject("window");
	if (window_object == nullptr) { return false; }

	m_screen_size = App->getConfig().GetIntFromJSONObject(window_object, "screen_size");
	if (m_screen_size == 0) { return false;  }

	return true;
}