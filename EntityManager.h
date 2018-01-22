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
	UpdateStatus Update(float dt = 0.0f) override;
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
	float time_left_msec = 81999.0f;

private:
	std::list<Entity*> entities;
	
	const float logicLoopUpdatePeriodMsec = 20.0f;
	float logicLoopAccumMsec = 0.0f;

	//collision check
	int layer_depth = 0;
	
	// Debug mode
	bool debug = false;

};

#endif // __ENTITYMANAGER_H__
