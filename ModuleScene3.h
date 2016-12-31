#ifndef __MODULESCENE3_H__
#define __MODULESCENE3_H__

#include "Module.h"
#include "Point.h"

class Animation;
struct SDL_Texture;
class Player;

class ModuleScene3 : public Module 
{

public:

	ModuleScene3(bool active = true);
	~ModuleScene3();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	
	bool LoadConfigFromFile(const char* file_path);


public:
	Player* player;

private:

	SDL_Texture* graphics = nullptr;
	
	SDL_Rect background_section;
	iPoint background_pos = iPoint(0, 0);

	SDL_Rect middleground_section;
	iPoint middleground_pos = iPoint(0, 0);

	SDL_Rect foreground_section;
	iPoint foreground_pos = iPoint(0, 0);

	Animation wave_sand;
	iPoint wave_sand_pos = iPoint(0, 0);

	Animation wave_splash;
	iPoint wave_splash_pos = iPoint(0, 0);

	std::string music_path = "";
	unsigned int fx_waves = 0;

};


#endif // __MODULESCENE3_H__
