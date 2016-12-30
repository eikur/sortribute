#include "Globals.h"
#include "EntityManager.h"
#include "Player.h"

EntityManager::EntityManager(){}

EntityManager::~EntityManager() {}

update_status EntityManager::Update() {
	return UPDATE_CONTINUE;
}


Entity* EntityManager::CreateEntity(Entity::Types type)
{
	static_assert(Entity::Types::unknown == 1, "code needs update");
	Entity* ret = nullptr;
	
	switch (type)
	{
		case Entity::Types::player: ret = new Player(); break;
	}

	if (ret != nullptr)
	{
		if (ret->Init() == false)
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			entities.push_back(ret);
		}
	}

	return ret;
}
