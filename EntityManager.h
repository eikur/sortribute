#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <list>
#include "Module.h"
#include "Entity.h"

class Timer;
class Player;

struct Font;
struct SDL_Texture;

struct ptrEntityDepthComparison {
	bool operator() (const Entity* left, const Entity* right) const {
		return left->GetDepth() < right->GetDepth();
	}
};

class EntityManager: public Module {

public:
	EntityManager(bool active);
	~EntityManager();

	bool Init();
	update_status Update();
	bool CleanUp();

	Entity* CreateEntity(Entity::Types type);

private:
//	bool OrderEntities(const Entity& comp, const Entity& reference);

	void PrintStatus();
	bool LoadConfigFromFile(const char* file_path);
	void CheatCodes();

public:
	Entity* player = nullptr;

private:
	std::list<Entity*> entities;
	
	int time_left = 50;

	bool pause = false; 
	Uint32 elapsed_msec = 0;
	Uint32 upd_logic_msec = 20;
	bool upd_logic = false;

	//hud
	SDL_Texture *hud_graphics = nullptr;
	SDL_Rect hud_section = { 0,0,0,0 };
	iPoint hud_time_pos = { 0,0 };
	iPoint hud_score_pos = { 0,0 };
	iPoint hud_help_pos = { 0,0 };
	iPoint hud_lives_pos = { 0,0 };

};

#endif // __ENTITYMANAGER_H__
