#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <list>
#include "Module.h"
#include "Entity.h"

class EntityManager: public Module {

public:
	EntityManager();
	~EntityManager();

	Entity* CreateEntity(Entity::Types type);

private:
	std::list<Entity*> entities;

};

#endif // __ENTITYMANAGER_H__
