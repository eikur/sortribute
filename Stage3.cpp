#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "TextureHelper.h"
#include "ModuleAudio.h"
#include "Animation.h"
#include "ModuleWindow.h"
#include "EntityManager.h"
#include "ModuleCollision.h"
#include "ModuleUI.h"
#include "SceneIntro.h"

#include "Stage3.h"

Stage3::Stage3(SceneManager& manager) : Scene(manager)
{
}

Stage3::~Stage3()
{}

bool Stage3::Init()
{
	LOG("Scene3: Starting MoonBeach\n");

	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Scene3: Unable to load config from file\n");
		return false;
	}

	return true;
}

bool Stage3::Start()
{
	CreateSceneTriggers();
	PlaceSceneItems();

	App->getAudio().PlayMusic(music_path.c_str(), 1.0F);	
	App->getAudio().PlayFx(fx_waves);
	
	App->getEntityManager().Enable();
	App->getEntityManager().player = App->getEntityManager().CreateEntity(Entity::Types::player);

	App->getUI().Enable();
	
	App->getRenderer().locked = false;

	return true;
}



UpdateStatus Stage3::PreUpdate()
{
	//Draw everything except wave_splash
	App->getRenderer().Blit(graphics, background_pos.x, background_pos.y, &background_section, (float)(background_section.w - App->getWindow().m_screen_width) / (float)(foreground_section.w - App->getWindow().m_screen_width));
	App->getRenderer().Blit(graphics, middleground_pos.x, middleground_pos.y, &middleground_section, (float)(middleground_section.w - App->getWindow().m_screen_width) / (float)(foreground_section.w - App->getWindow().m_screen_width));
	App->getRenderer().Blit(graphics, foreground_pos.x, foreground_pos.y, &foreground_section, 1.0F);
	App->getRenderer().Blit(graphics, wave_sand_pos.x, wave_sand_pos.y, &(wave_sand.GetCurrentFrame()), 1.0F);			
	return UpdateStatus::Continue;
}
UpdateStatus Stage3::Update(float)
{
	App->getRenderer().Blit(graphics, wave_splash_pos.x, wave_splash_pos.y, &(wave_splash.GetCurrentFrame()), 1.0F);

	if (battle_zone4 == nullptr && App->getEntityManager().boss == nullptr)
	{
		getManager().SwapScene(SceneManager::SceneId::Intro);
	}
	return UpdateStatus::Continue;
}

bool Stage3::CleanUp()
{
	LOG("Scene3: Unloading Moon Beach scene\n");
	DeleteSceneTriggers();

	return true;
}

void Stage3::HandleCollision(Collider* a, Collider* b)
{
	if (a->type)
		TriggerCollisionManagement(a);
	else
		TriggerCollisionManagement(b);
}

void Stage3::CreateSceneTriggers()
{
	triggers.push_back(spawn1 = App->getCollision().AddCollider({ 300,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn2 = App->getCollision().AddCollider({ 450,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn3 = App->getCollision().AddCollider({ 520,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn4 = App->getCollision().AddCollider({ 960,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn5 = App->getCollision().AddCollider({ 1100,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn6 = App->getCollision().AddCollider({ 1570,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn7 = App->getCollision().AddCollider({ 1926,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn8 = App->getCollision().AddCollider({ 2082,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn9 = App->getCollision().AddCollider({ 2608,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(spawn10 = App->getCollision().AddCollider({ 2706,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));

	triggers.push_back(cam_lock1 = App->getCollision().AddCollider({ 590,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(battle_zone1 = App->getCollision().AddCollider({ 450,130,280,94 }, colliderType::SCENE_TRIGGER, nullptr));
	
	triggers.push_back(cam_lock2 = App->getCollision().AddCollider({ 1360,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(battle_zone2 = App->getCollision().AddCollider({ 1220,130,280,94 }, colliderType::SCENE_TRIGGER, nullptr));

	triggers.push_back(cam_lock3 = App->getCollision().AddCollider({ 2130,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(battle_zone3 = App->getCollision().AddCollider({ 1990,130,280,94 }, colliderType::SCENE_TRIGGER, nullptr));

	triggers.push_back(cam_lock4 = App->getCollision().AddCollider({ 2900,32,5,192 }, colliderType::SCENE_TRIGGER, nullptr));
	triggers.push_back(battle_zone4 = App->getCollision().AddCollider({ 2760,130,280,94 }, colliderType::SCENE_TRIGGER, nullptr));
}

void Stage3::DeleteSceneTriggers()
{
	for (std::vector<Collider*>::iterator it = triggers.begin(); it != triggers.end(); ++it)
	{
		if ((*it) != nullptr) {
			(*it)->to_delete = true;
			*it = nullptr;
		}
	}
	triggers.clear();
}


void Stage3::PlaceSceneItems()
{
	Entity *tmp = nullptr;
	tmp = App->getEntityManager().CreateEntity(Entity::Types::item_apple);
	tmp->SetPosition({ 400,180 });
	tmp = App->getEntityManager().CreateEntity(Entity::Types::item_apple);
	tmp->SetPosition({ 1155,180 });
	tmp = App->getEntityManager().CreateEntity(Entity::Types::item_chicken);
	tmp->SetPosition({1824,170});
	tmp = App->getEntityManager().CreateEntity(Entity::Types::item_apple);
	tmp->SetPosition({ 1950,170 });
}

void Stage3::TriggerCollisionManagement(Collider *trigger)
{
	if (trigger == nullptr)
		return;

	Entity *tmp = nullptr;
	
	if (trigger == spawn1)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn1->rect.x -170, 200 });
		spawn1 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == spawn2)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp!= nullptr)
			tmp->SetPosition({ spawn2->rect.x + 170, 216 });
		spawn2 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == spawn3)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn3->rect.x + 170, 162 });
		spawn3 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == cam_lock1)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock1->rect.x -180, 162 });
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock1->rect.x +170 , 210 });
		App->getRenderer().locked = true;
		cam_lock1 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == battle_zone1)
	{
		if (cam_lock1 == nullptr && App->getEntityManager().GetEnemyCount() == 0)
		{
			App->getRenderer().locked = false;
			App->getAudio().PlayFx(fx_waves);
			tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
			if (tmp != nullptr)
				tmp->SetPosition({battle_zone1->rect.x +battle_zone1->rect.w + 340, 210});
			App->getEntityManager().RestoreTimeLeft();
			App->getUI().ShowGoArrow();
			battle_zone1 = nullptr;
			trigger->to_delete = true;
		}
	}
	else if (trigger == spawn4)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn4->rect.x + 170, 162 });
		spawn4 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == spawn5)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn5->rect.x -170, 216 });
		spawn5 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == cam_lock2)
	{
		App->getRenderer().locked = true;
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock2->rect.x - 170, 162 });
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock2->rect.x + 250, 210 });
		cam_lock2 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == battle_zone2)
	{
		if (cam_lock2 == nullptr && App->getEntityManager().GetEnemyCount() == 0)
		{
			App->getRenderer().locked = false;
			App->getAudio().PlayFx(fx_waves);
			tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
			if (tmp != nullptr)
				tmp->SetPosition({ battle_zone2->rect.x + battle_zone2->rect.w + 300, 210 });
			App->getEntityManager().RestoreTimeLeft();
			App->getUI().ShowGoArrow();
			battle_zone2 = nullptr;
			trigger->to_delete = true;
		}
	}
	else if (trigger == spawn6)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn6->rect.x + 180, 210 });
		spawn6 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == spawn7)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn7->rect.x + 180, 175 });
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn7->rect.x + 250, 175 });
		spawn7 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == spawn8)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn8->rect.x + 170, 210 });
		spawn8 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == cam_lock3)
	{
		App->getRenderer().locked = true;
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock3->rect.x - 180, 162 });
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock3->rect.x + 250, 210 });
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock3->rect.x - 340, 202 });
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock3->rect.x + 420, 170 });
		cam_lock3 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == battle_zone3)
	{
		if (cam_lock3 == nullptr && App->getEntityManager().GetEnemyCount() == 0)
		{
			App->getRenderer().locked = false;
			App->getAudio().PlayFx(fx_waves);
			App->getEntityManager().RestoreTimeLeft();
			App->getUI().ShowGoArrow();
			battle_zone3 = nullptr;
			trigger->to_delete = true;
		}
	}
	else if (trigger == spawn9)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn9->rect.x + 180, 180 });
		spawn9 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == spawn10)
	{
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ spawn10->rect.x + 180, 210 });
		spawn10 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == cam_lock4)
	{
		App->getRenderer().locked = true;
		tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_garcia);
		if (tmp != nullptr)
			tmp->SetPosition({ cam_lock4->rect.x - 180, 210 });
		cam_lock4 = nullptr;
		trigger->to_delete = true;
	}
	else if (trigger == battle_zone4)
	{
		if (cam_lock4 == nullptr && App->getEntityManager().GetEnemyCount() == 0)
		{
			App->getAudio().PlayMusic(boss_music_path.c_str(),0.0F);
			App->getEntityManager().RestoreTimeLeft();
			do {
				tmp = App->getEntityManager().CreateEntity(Entity::Types::npc_boss);
			} while (tmp == nullptr);
			tmp->SetPosition({ battle_zone4->rect.x + battle_zone4->rect.w + 300, 180 });
			App->getEntityManager().boss = tmp;
			battle_zone4 = nullptr;
			trigger->to_delete = true;
		}
	}

}

bool Stage3::LoadConfigFromFile(const char* file_path)
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
		graphics = App->getTextures().Load(json_object_dotget_string(root_object, "scene3.graphics_file"));
	
	if (json_object_dothas_value_of_type(root_object, "scene3.music_file",JSONString))
		music_path = json_object_dotget_string(root_object, "scene3.music_file");

	if (json_object_dothas_value_of_type(root_object, "scene3.boss_music_file", JSONString))
		boss_music_path = json_object_dotget_string(root_object, "scene3.boss_music_file");
	
	if (json_object_dothas_value_of_type(root_object, "scene3.fx_waves", JSONString))
		fx_waves = App->getAudio().LoadFx(json_object_dotget_string(root_object, "scene3.fx_waves"));


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
		return false;
	}
	
	return true;
}