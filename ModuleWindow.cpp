#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

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
	else if (LoadConfigFromFile("config.json") == false)
	{
		LOG("Configuration load from JSON file failed\n");
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
bool ModuleWindow::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	m_screen_width = json_object_dotget_number(json_object(root_value), "window.screen_width");
	m_screen_height = json_object_dotget_number(json_object(root_value), "window.screen_height");
	m_screen_size = json_object_dotget_number(json_object(root_value), "window.screen_size");
	m_fullscreen = json_object_dotget_boolean(json_object(root_value), "window.fullscreen");
	m_vsync = json_object_dotget_boolean(json_object(root_value), "window.vsync");
	m_title = json_object_dotget_string(json_object(root_value), "window.title");
	json_value_free(root_value);
	if (m_screen_width == 0 || m_screen_height == 0 || m_screen_size == 0)
		return false;
	else 
		return true;
}