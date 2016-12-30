#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <list>
#include "Module.h"
#include "Entity.h"

class Timer;

struct Font;
struct SDL_Texture;

class EntityManager: public Module {

public:
	EntityManager();
	~EntityManager();

	bool Init();
	update_status Update();
	bool CleanUp();

	Entity* CreateEntity(Entity::Types type);
	void SetTimeLeft(int new_time);

	bool LoadConfigFromFile(const char* file_path); 

private:
	int time_left = 50;
	iPoint hud_time_pos = iPoint(0, 0);

	std::list<Entity*> entities;
	
	Timer *logic_timer = nullptr;
	unsigned int upd_logic_ms_cycle = 20;
	bool upd_logic = false;

};

#endif // __ENTITYMANAGER_H__
