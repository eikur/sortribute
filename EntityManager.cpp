#include "Globals.h"
#include "EntityManager.h"
#include "Player.h"

EntityManager::EntityManager(){}

EntityManager::~EntityManager() {}

Entity* EntityManager::CreateEntity(Entity::Types type)
{
	static_assert(Entity::Types::unknown == 1, "code needs update");
	Entity* ret = nullptr;
	
	switch (type)
	{
		case Entity::Types::player: ret = new Player(); break;
	}

	if (ret != nullptr)
		entities.push_back(ret);

	return ret;
}
