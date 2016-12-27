#ifndef __MODULESCENE3_H__
#define __MODULESCENE3_H__

#include "Module.h"
struct SDL_Texture;

class ModuleScene3 : Module 
{
public:
	ModuleScene3(bool active = true);
	~ModuleScene3();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();
	
	bool LoadConfigFromFile(const char* file_path);

private:
	SDL_Texture* graphics = nullptr;
	uint music = 0;
};


#endif // __MODULESCENE3_H__
