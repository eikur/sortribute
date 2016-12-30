#include "Globals.h"
#include "EntityManager.h"
#include "Player.h"

EntityManager::EntityManager(){}

EntityManager::~EntityManager() {}

update_status EntityManager::Update() {
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->Update();
	return UPDATE_CONTINUE;
}

bool EntityManager::CleanUp()
{
	LOG("EntityManager: Removing entities from application\n");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;
	entities.clear();

	return true;
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
