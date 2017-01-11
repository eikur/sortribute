#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"

struct SDL_Texture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool active = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	bool LoadConfigFromFile(const char *file_path);

public:
	
	SDL_Texture* background = nullptr;
};

#endif // __MODULESCENEINTRO_H__