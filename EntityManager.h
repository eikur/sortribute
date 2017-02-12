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
	bool Start();
	update_status Update();
	bool CleanUp();

	Entity* CreateEntity(Entity::Types type);
	void KnockDownAllEnemies(bool wipeout = false);
	int GetEnemyCount();
	void RestoreTimeLeft();

private:
	void HandleCollision(Collider* a, Collider* b);

	bool LoadConfigFromFile(const char* file_path);
	void CheatCodes();

public:
	Entity* player = nullptr;
	Entity* boss = nullptr;
	std::list<Entity*> enemy_queue;
	int time_left_msec = 81999;

private:
	std::list<Entity*> entities;
	
	Uint32 elapsed_msec = 0;
	Uint32 upd_logic_msec = 20;
	bool upd_logic = false;

	//collision check
	int layer_depth = 0;
};

#endif // __ENTITYMANAGER_H__
