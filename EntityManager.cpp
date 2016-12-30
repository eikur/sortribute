#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Player.h"
#include "Timer.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"

#include "EntityManager.h"

EntityManager::EntityManager(){

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

update_status EntityManager::PreUpdate()
{
	CheatCodes();
	return UPDATE_CONTINUE;
}

update_status EntityManager::Update() 
{
	// draw HUD
	App->renderer->Blit(hud_graphics, 0, 0, &hud_section, 0.0F);
	App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, GetPrintableValue(score, 6));
	App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, GetPrintableValue(help,1));
	App->fonts->Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, GetPrintableValue(time_left, 2));
	App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, GetPrintableValue(lives, 1));

	// Update all entities
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
	
	App->textures->Unload(hud_graphics);
	
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

	if (json_object_dothas_value_of_type(root_object, "manager.hud.graphics_file", JSONString))
		hud_graphics = App->textures->Load(json_object_dotget_string(root_object, "manager.hud.graphics_file"));
	if (hud_graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

	j_array = json_object_dotget_array(root_object, "manager.hud.section");
	hud_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "manager.hud.score_pos");
	hud_score_pos.x = (int)json_array_get_number(j_array, 0);
	hud_score_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "manager.hud.time_pos");
	hud_time_pos.x = (int)json_array_get_number(j_array, 0);
	hud_time_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "manager.hud.lives_pos");
	hud_lives_pos.x = (int)json_array_get_number(j_array, 0);
	hud_lives_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "manager.hud.help_pos");
	hud_help_pos.x = (int)json_array_get_number(j_array, 0);
	hud_help_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	
	json_value_free(root_value);
	return true;
}

std::string EntityManager::GetPrintableValue(int value, int desired_length)
{
	std::string ret = "";
	std::string tmp = std::to_string(value);

	int current_length = tmp.length();

	for (int i = current_length; i < desired_length; ++i)
	{
		ret.append("0");
	}
	ret.append(tmp);
	return ret;
}

void EntityManager::SetTimeLeft(int new_time) {
	time_left = new_time;
}

void EntityManager::CheatCodes() {
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
		score += 125;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && lives < 9)
		lives += 1;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && help < 9 )
		help += 1;
}