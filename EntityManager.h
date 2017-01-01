#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <list>
#include "Module.h"
#include "Entity.h"

class Timer;
class Player;

struct Font;
struct SDL_Texture;

class EntityManager: public Module {

public:
	EntityManager(bool active);
	~EntityManager();

	bool Init();
	update_status Update();
	bool CleanUp();

	Entity* CreateEntity(Entity::Types type);
	int GetPlayerXPos();

	void PrintStatus();
	bool LoadConfigFromFile(const char* file_path); 

public:
	Entity* player = nullptr;

private:
	std::list<Entity*> entities;
	
	int time_left = 50;

	bool pause = false; 
	Timer *logic_timer = nullptr;
	int elapsed_msec = 0;
	int upd_logic_msec = 10;
	bool upd_logic = false;

	//hud
	SDL_Texture *hud_graphics = nullptr;
	SDL_Rect hud_section = SDL_Rect();
	iPoint hud_time_pos = iPoint(0, 0);
	iPoint hud_score_pos = iPoint(0, 0);
	iPoint hud_help_pos = iPoint(0, 0);
	iPoint hud_lives_pos = iPoint(0, 0);

};

#endif // __ENTITYMANAGER_H__
