#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed = 1.0f);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

	bool LoadConfigFromFile(const char* file_path);

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
private:
	int m_screen_height = 0;
	int m_screen_size = 0;
	int m_screen_width = 0;
	bool m_vsync = true;

};

#endif // __MODULERENDER_H__