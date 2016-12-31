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
	void SetTimeLeft(int new_time);

	void PrintStatus() const;
	bool LoadConfigFromFile(const char* file_path); 

public:
	Entity* player = nullptr;

private:
	std::list<Entity*> entities;
	
	int time_left = 50;


	Timer *logic_timer = nullptr;
	unsigned int elapsed_msec = 0;
	unsigned int upd_logic_msec = 50;
	bool upd_logic = false;

	iPoint hud_time_pos = iPoint(0, 0);
	iPoint hud_score_pos = iPoint(0, 0);
	iPoint hud_help_pos = iPoint(0, 0);
	iPoint hud_lives_pos = iPoint(0, 0);

};

#endif // __ENTITYMANAGER_H__
