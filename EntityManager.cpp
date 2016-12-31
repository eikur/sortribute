#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Player.h"
#include "Timer.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"

#include "EntityManager.h"

EntityManager::EntityManager(bool active): Module(active){

}

EntityManager::~EntityManager() {
}

bool EntityManager::Init()
{
	logic_timer = new Timer();
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Entity Manager: failed to initialise\n");
		return false;
	}
	else 
		return true;
}

update_status EntityManager::Update() 
{
	PrintStatus();

	//Elapsed time
	unsigned int msec_increase = logic_timer->ElapsedTimeMsec() - elapsed_msec;
	elapsed_msec += msec_increase;
	if (elapsed_msec >= upd_logic_msec)
		upd_logic = true;

	//Update all entities
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->Update(elapsed_msec, upd_logic );

	if (upd_logic == true)
	{
		logic_timer->ReStart();
		elapsed_msec = 0;
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

bool EntityManager::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;
	JSON_Array *j_array;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	j_array = json_object_dotget_array(root_object, "hud.time_pos");
	hud_time_pos.x = (int)json_array_get_number(j_array, 0);
	hud_time_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.score_pos");
	hud_score_pos.x = (int)json_array_get_number(j_array, 0);
	hud_score_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);


	j_array = json_object_dotget_array(root_object, "hud.lives_pos");
	hud_lives_pos.x = (int)json_array_get_number(j_array, 0);
	hud_lives_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.help_pos");
	hud_help_pos.x = (int)json_array_get_number(j_array, 0);
	hud_help_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);


	json_value_free(root_value);
	return true;
}


void EntityManager::PrintStatus() const
{
	// draw HUD
	App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, App->fonts->GetPrintableValue(player->score , 6));
	App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(player->help, 1));
	App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(player->lives, 1));
	App->fonts->Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(time_left, 2));
}

void EntityManager::SetTimeLeft(int new_time) {
	time_left = new_time;
}

