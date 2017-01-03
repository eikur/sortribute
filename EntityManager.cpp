#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleTimer.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"

#include "Player.h"
#include "EnemyGarcia.h"

#include "EntityManager.h"

EntityManager::EntityManager(bool active): Module(active){

}

EntityManager::~EntityManager() {
}

bool EntityManager::Init()
{
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Entity Manager: failed to initialise\n");
		return false;
	}
	else 
		return true;
}

Entity* EntityManager::CreateEntity(Entity::Types type)
{
	static_assert(Entity::Types::unknown == 2, "code needs update");
	Entity* ret = nullptr;

	switch (type)
	{
	case Entity::Types::player: ret = new Player(this); break;
	case Entity::Types::npc_garcia: ret = new EnemyGarcia(this); break;
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

update_status EntityManager::Update() 
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		pause = !pause;
		if (pause)
			App->timer->TimerPause();
		else
			App->timer->TimerResume();
	}

	if (pause)
	{
		
	}
	else
	{
		elapsed_msec += App->timer->DeltaTime();

		if (elapsed_msec >= upd_logic_msec)
			upd_logic = true;

		for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Update(elapsed_msec, upd_logic);
		
		entities.sort(Entity::ptrEntityDepthComparison());
		for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Draw();

		if (upd_logic == true)
		{
			elapsed_msec = 0;
			upd_logic = false;
		}
	}

	PrintStatus();
	CheatCodes();

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

void EntityManager::PrintStatus() 
{
	App->renderer->Blit(hud_graphics, 0, 0, &hud_section, 0.0F);
	App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, App->fonts->GetPrintableValue(player->score, 6));
	App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(player->help, 1));
	App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(player->lives, 1));
	App->fonts->Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(time_left, 2));
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

	// hud load
	if (json_object_dothas_value_of_type(root_object, "hud.graphics_file", JSONString))
		hud_graphics = App->textures->Load(json_object_dotget_string(root_object, "hud.graphics_file"));
	if (hud_graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

	j_array = json_object_dotget_array(root_object, "hud.section");
	hud_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

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

void EntityManager::CheatCodes()
{
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		Entity *a = (Entity*) CreateEntity(Entity::Types::npc_garcia);
		if (a != nullptr){
			a->position.y = player->position.y;	
			a->ground_y = player->position.y;
			a->position.x = player->position.x + 150;
		}
	}
}



