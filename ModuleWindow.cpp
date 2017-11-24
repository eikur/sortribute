#include "Globals.h"
#include "Application.h"
#include "ConfigurationLoader.h"

#include "ModuleWindow.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else if (LoadConfigFromFile() == false)
	{
		LOG("Window: Unable to load configuration from file\n");
		ret = false;
	}
	else
	{
		int width = m_screen_width * m_screen_size;
		int height = m_screen_height * m_screen_size;

		Uint32 flags = SDL_WINDOW_SHOWN;

		if(m_fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(m_window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			m_screen_surface = SDL_GetWindowSurface(m_window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Read parameters from config file
bool ModuleWindow::LoadConfigFromFile()
{
	JSON_Object *window_object = App->getConfig().GetJSONObject("window"); 
	if (window_object == nullptr) { return false;  }

	m_screen_width = App->getConfig().GetIntFromJSONObject(window_object, "screen_width"); 
	if (m_screen_width == 0) { return false;  }
	m_screen_height = App->getConfig().GetIntFromJSONObject(window_object, "screen_height");
	if (m_screen_height == 0) { return false; }
	m_screen_size = App->getConfig().GetIntFromJSONObject(window_object, "screen_size");
	if (m_screen_size == 0) { return false;  }
	
	m_fullscreen = App->getConfig().GetBoolFromJSONObject(window_object, "fullscreen"); 
	m_vsync = App->getConfig().GetBoolFromJSONObject(window_object, "vsync");

	m_title = App->getConfig().GetStringFromJSONObject(window_object, "title"); 

	return true;
}