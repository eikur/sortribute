#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <list>
#include "Module.h"
#include "Entity.h"
#include <math.h>

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
	void KnockDownAllEnemies();
	int GetEnemyCount();

private:
	void HandleCollision(Collider* a, Collider* b);

	void PrintStatus();
	void PrintPlayerHealth();
	void PrintBossHealth();

	bool LoadConfigFromFile(const char* file_path);
	void CheatCodes();

public:
	Entity* player = nullptr;
	Entity* boss = nullptr;
	std::list<Entity*> enemy_queue;

private:
	std::list<Entity*> entities;
	
	int time_left_msec = 100000;
	bool pause = false; 
	Uint32 elapsed_msec = 0;
	Uint32 upd_logic_msec = 20;
	bool upd_logic = false;
	unsigned int fx_pause = 0;

	//hud
	SDL_Texture *hud_graphics = nullptr;
	SDL_Rect hud_section = { 0,0,0,0 };
	iPoint hud_score_pos = { 0,0 };
	iPoint hud_health_pos = { 0,0 };
	SDL_Rect hud_health_section = { 0,0,0,0 };
	iPoint hud_time_pos = { 0,0 };
	iPoint hud_help_pos = { 0,0 };
	iPoint hud_lives_pos = { 0,0 };
	
	SDL_Rect hud_boss_section = { 0,0,0,0 };
	SDL_Rect hud_high_health_section = { 0,0,0,0 };
	SDL_Rect hud_medium_health_section = { 0,0,0,0 };
	iPoint hud_boss_pos = { 0,0 };
	iPoint hud_boss_msg_pos = { 0,0 };
	iPoint hud_health_boss_pos = { 0,0 };
	
		
	//collision check
	int layer_depth = 0;
};

#endif // __ENTITYMANAGER_H__
