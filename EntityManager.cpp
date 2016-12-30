#include "Globals.h"
#include "EntityManager.h"
#include "Player.h"
#include "Timer.h"

EntityManager::EntityManager(){

}

EntityManager::~EntityManager() {
}

bool EntityManager::Init()
{
	logic_timer = new Timer();
	return true;
}
update_status EntityManager::Update() {

	unsigned int a = logic_timer->ElapsedTimeMsec();
	if (logic_timer->ElapsedTimeMsec() >= upd_logic_ms_cycle)
		upd_logic = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->Update( upd_logic );

	if (upd_logic == true)
	{
		logic_timer->ReStart();
		upd_logic = false; 
	}

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
	delete logic_timer;

	return ret;
}
