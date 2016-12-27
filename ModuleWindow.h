#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
private:
	int m_screen_width = 0;
	int m_screen_height = 0; 
	int m_screen_size = 0;
	bool m_fullscreen = false;
	bool m_vsync = false; 
	std::string m_title = "";

public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before starting
	bool Init();

	// Called before quitting
	bool CleanUp();

	bool LoadConfigFromFile(const char* file_path);

public:
	//The window we'll be rendering to
	SDL_Window* m_window = nullptr;

	//The surface contained by the window
	SDL_Surface* m_screen_surface = nullptr;
};

#endif // __MODULEWINDOW_H__