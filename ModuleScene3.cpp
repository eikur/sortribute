#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "Animation.h"
#include "ModuleWindow.h"
#include "EntityManager.h"
#include "ModuleCollision.h"

#include "ModuleScene3.h"

ModuleScene3::ModuleScene3(bool active) : Module(active)
{
}

ModuleScene3::~ModuleScene3()
{}


bool ModuleScene3::Init()
{
	LOG("Scene3: Init from config file\n");

	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Scene3: Unable to load config from file\n");
		return false;
	}
	CreateSceneTriggers();
	PlaceSceneItems();

	return true;
}

bool ModuleScene3::Start()
{
	LOG("Scene3: Starting MoonBeach\n");
	App->audio->PlayMusic(music_path.c_str(), 1.0F);	
	App->audio->PlayFx(fx_waves);
	App->manager->Enable();
	App->manager->player = App->manager->CreateEntity(Entity::Types::player);
	
	return true;
}



update_status ModuleScene3::PreUpdate()
{
	//Draw everything except wave_splash
	App->renderer->Blit(graphics, background_pos.x, background_pos.y, &background_section, (float)(background_section.w - App->window->m_screen_width) / (float)(foreground_section.w - App->window->m_screen_width));
	App->renderer->Blit(graphics, middleground_pos.x, middleground_pos.y, &middleground_section, (float)(middleground_section.w - App->window->m_screen_width) / (float)(foreground_section.w - App->window->m_screen_width));
	App->renderer->Blit(graphics, foreground_pos.x, foreground_pos.y, &foreground_section, 1.0F);
	App->renderer->Blit(graphics, wave_sand_pos.x, wave_sand_pos.y, &(wave_sand.GetCurrentFrame()), 1.0F);			
	return UPDATE_CONTINUE;
}
update_status ModuleScene3::Update()
{
	App->renderer->Blit(graphics, wave_splash_pos.x, wave_splash_pos.y, &(wave_splash.GetCurrentFrame()), 1.0F);	// waves painted last
	
	return UPDATE_CONTINUE;
}

bool ModuleScene3::CleanUp()
{
	LOG("Scene3: Unloading Moon Beach scene\n");
	App->textures->Unload(graphics);

	return true;
}

void ModuleScene3::HandleCollision(Collider* a, Collider* b)
{
	if (a->type)
		TriggerCollisionManagement(a);
	else
		TriggerCollisionManagement(b);
}

void ModuleScene3::CreateSceneTriggers()
{
	triggers.push_back(spawn1 = App->collision->AddCollider({ 300,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn2 = App->collision->AddCollider({ 450,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn3 = App->collision->AddCollider({ 520,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(cam_lock1 = App->collision->AddCollider({ 590,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(battle_zone1 = App->collision->AddCollider({ 450,130,260,94 }, colliderType::SCENE_TRIGGER, nullptr));

	triggers.push_back(cam_lock2 = App->collision->AddCollider({ 590,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(battle_zone2 = App->collision->AddCollider({ 450,130,260,94 }, colliderType::SCENE_TRIGGER, nullptr));

	triggers.push_back(cam_lock3 = App->collision->AddCollider({ 590,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(battle_zone3 = App->collision->AddCollider({ 450,130,260,94 }, colliderType::SCENE_TRIGGER, nullptr));

	triggers.push_back(cam_lock4 = App->collision->AddCollider({ 590,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(battle_zone4 = App->collision->AddCollider({ 450,130,260,94 }, colliderType::SCENE_TRIGGER, nullptr));

}

void ModuleScene3::PlaceSceneItems()
{
	Entity *tmp = App->manager->CreateEntity(Entity::Types::item_apple);
	tmp->SetPosition({ 420,170 });
}

void ModuleScene3::TriggerCollisionManagement(Collider *trigger)
{
	Entity *tmp = nullptr;
	if (trigger == spawn1)
	{
		tmp = App->manager->CreateEntity(Entity::Types::npc_garcia);
		tmp->SetPosition({ 130, 200 });
		spawn1 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == spawn2)
	{
		tmp = App->manager->CreateEntity(Entity::Types::npc_garcia);
		tmp->SetPosition({ 620, 200 });
		spawn2 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == spawn3)
	{
		tmp = App->manager->CreateEntity(Entity::Types::npc_garcia);
		tmp->SetPosition({ 690, 162 });
		spawn3 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == cam_lock1)
	{
		tmp = App->manager->CreateEntity(Entity::Types::npc_garcia);
		tmp->SetPosition({ 320, 150 });
		tmp = App->manager->CreateEntity(Entity::Types::npc_garcia);
		tmp->SetPosition({ 800, 210 });
		App->renderer->locked = true;
		cam_lock1 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == battle_zone1)
	{
		if (cam_lock1 == nullptr && App->manager->GetEnemyCount() == 0)
		{
			App->renderer->locked = false;
			tmp = App->manager->CreateEntity(Entity::Types::npc_garcia);
			tmp->SetPosition({800, 210});
			battle_zone1 = nullptr;
			trigger->to_delete = true;
		}
	}
}

bool ModuleScene3::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;
	JSON_Array *j_array_pos; 
	JSON_Array *j_array_section;
	JSON_Array *j_array_tmp;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	if (json_object_dothas_value_of_type(root_object, "scene3.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(root_object, "scene3.graphics_file"));
	
	if (json_object_dothas_value_of_type(root_object, "scene3.music_file",JSONString))
		music_path = json_object_dotget_string(root_object, "scene3.music_file");

	if (json_object_dothas_value_of_type(root_object, "scene3.fx_waves", JSONString))
		fx_waves = App->audio->LoadFx(json_object_dotget_string(root_object, "scene3.fx_waves"));


	//background load
	j_array_pos = json_object_dotget_array(root_object, "scene3.background.position");
	j_array_section = json_object_dotget_array(root_object, "scene3.background.section");

	background_pos.x = (int)json_array_get_number(j_array_pos, 0);
	background_pos.y = (int)json_array_get_number(j_array_pos, 1);
	background_section.x = (int)json_array_get_number(j_array_section, 0);
	background_section.y = (int)json_array_get_number(j_array_section, 1);
	background_section.w = (int)json_array_get_number(j_array_section, 2);
	background_section.h= (int)json_array_get_number(j_array_section, 3);

	json_array_clear(j_array_pos);
	json_array_clear(j_array_section);

	//middleground load 
	j_array_pos = json_object_dotget_array(root_object, "scene3.middleground.position");
	j_array_section = json_object_dotget_array(root_object, "scene3.middleground.section");

	middleground_pos.x = (int)json_array_get_number(j_array_pos, 0);
	middleground_pos.y = (int)json_array_get_number(j_array_pos, 1);
	middleground_section.x = (int)json_array_get_number(j_array_section, 0);
	middleground_section.y = (int)json_array_get_number(j_array_section, 1);
	middleground_section.w = (int)json_array_get_number(j_array_section, 2);
	middleground_section.h = (int)json_array_get_number(j_array_section, 3);

	json_array_clear(j_array_pos);
	json_array_clear(j_array_section);

	//foreground load
	j_array_pos = json_object_dotget_array(root_object, "scene3.foreground.position");
	j_array_section = json_object_dotget_array(root_object, "scene3.foreground.section");

	foreground_pos.x = (int)json_array_get_number(j_array_pos, 0);
	foreground_pos.y = (int)json_array_get_number(j_array_pos, 1);
	foreground_section.x = (int)json_array_get_number(j_array_section, 0);
	foreground_section.y = (int)json_array_get_number(j_array_section, 1);
	foreground_section.w = (int)json_array_get_number(j_array_section, 2);
	foreground_section.h = (int)json_array_get_number(j_array_section, 3);

	json_array_clear(j_array_pos);
	json_array_clear(j_array_section);

	//wave_sand load
	j_array_pos = json_object_dotget_array(root_object, "scene3.wave_sand.position");
	j_array_section = json_object_dotget_array(root_object, "scene3.wave_sand.section");

	wave_sand_pos.x = (int)json_array_get_number(j_array_pos, 0);
	wave_sand_pos.y = (int)json_array_get_number(j_array_pos, 1);
	
	for (int i = 0; i < (int) json_array_get_count(j_array_section); ++i)
	{
		j_array_tmp = json_array_get_array(j_array_section, i);
		wave_sand.frames.push_back({ (int)json_array_get_number(j_array_tmp, 0), (int)json_array_get_number(j_array_tmp, 1), (int)json_array_get_number(j_array_tmp, 2), (int)json_array_get_number(j_array_tmp, 3) });
		json_array_clear(j_array_tmp);
	}

	wave_sand.speed = (float)json_object_dotget_number(root_object, "scene3.wave_sand.speed");

	json_array_clear(j_array_pos);
	json_array_clear(j_array_section);

	//wave_splash load
	j_array_pos = json_object_dotget_array(root_object, "scene3.wave_splash.position");
	j_array_section = json_object_dotget_array(root_object, "scene3.wave_splash.section");

	wave_splash_pos.x = (int)json_array_get_number(j_array_pos, 0);
	wave_splash_pos.y = (int)json_array_get_number(j_array_pos, 1);

	for (int i = 0; i < (int)json_array_get_count(j_array_section); ++i)
	{
		j_array_tmp = json_array_get_array(j_array_section, i);
		wave_splash.frames.push_back({ (int)json_array_get_number(j_array_tmp, 0), (int)json_array_get_number(j_array_tmp, 1), (int)json_array_get_number(j_array_tmp, 2), (int)json_array_get_number(j_array_tmp, 3) });
		json_array_clear(j_array_tmp);
	}

	wave_splash.speed = (float)json_object_dotget_number(root_object, "scene3.wave_splash.speed");

	json_array_clear(j_array_pos);
	json_array_clear(j_array_section);

	// clean all and exit
	json_value_free(root_value);

	if (graphics == nullptr || music_path == "")
	{
		if (graphics != nullptr)
			App->textures->Unload(graphics);
		return false;
	}
	
	return true;
}