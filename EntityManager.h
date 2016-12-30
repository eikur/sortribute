#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <list>
#include "Module.h"
#include "Entity.h"
class Timer;

class EntityManager: public Module {

public:
	EntityManager();
	~EntityManager();

	bool Init();
	update_status Update();
	bool CleanUp();

	Entity* CreateEntity(Entity::Types type);

private:
	std::list<Entity*> entities;
	Timer *logic_timer = nullptr;
	unsigned int upd_logic_ms_cycle = 20;
	bool upd_logic = false;

};

#endif // __ENTITYMANAGER_H__
