#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "EntityManager.h"

#include "EnemyGarcia.h"

EnemyGarcia::EnemyGarcia(EntityManager* parent): Entity(Entity::Types::npc_garcia), parent(parent)
{ }

EnemyGarcia::~EnemyGarcia(){}

bool EnemyGarcia::Init() 
{
	if (LoadFromConfigFile(CONFIG_FILE) == false)
	{
		LOG("Error loading player config from file");
		return false;
	}
	UpdateCurrentAnimation(&idle);
	return true;
}

bool EnemyGarcia::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	if (upd_logic)
	{
		if (blocking_animation_remaining_msec > 0)
			blocking_animation_remaining_msec -= msec_elapsed;
		
		if (AllowAnimationInterruption())
		{
			if (parent->player->position.x >= position.x)
				facing_right = true;
			else
				facing_right = false;
			UpdatePosition({ 0,0 });
		}
	}
	UpdateCurrentAnimation(&idle);
	return UPDATE_CONTINUE;
}


void EnemyGarcia::Die() {
	// TODO: Implement death
}

bool EnemyGarcia::LoadFromConfigFile(const char* file_path) 
{
	JSON_Value *root_value;
	JSON_Object *root_object;
	JSON_Array *j_array;
	JSON_Array *j_array_inner;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	if (json_object_dothas_value_of_type(root_object, "garcia.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(root_object, "garcia.graphics_file"));
	if (graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}
	
//----------------------- colliders ---------------------------
	j_array = json_object_dotget_array(root_object, "garcia.colliders.hit");
	hit_collider_offset = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	hit_collider = App->collision->AddCollider(
	{ hit_collider_offset.x + position.x, hit_collider_offset.y + position.y, (int)json_array_get_number(j_array, 2) , (int)json_array_get_number(j_array, 3) },
		colliderType::ENEMY);

	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "garcia.colliders.attack");
	attack_collider_offset = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	attack_collider = App->collision->AddCollider(
	{ attack_collider_offset.x + position.x, attack_collider_offset.y + position.y, (int)json_array_get_number(j_array, 2) , (int)json_array_get_number(j_array, 3) },
		colliderType::ENEMY_ATTACK);
	json_array_clear(j_array);

//----------------------- animation duration ---------------------------
	attacks_duration = (int)json_object_dotget_number(root_object, "garcia.duration.attacks");
	being_hit_duration = (int)json_object_dotget_number(root_object, "garcia.duration.being_hit");
	being_knocked_duration = (int)json_object_dotget_number(root_object, "garcia.duration.being_knocked");
	being_thrown_duration = (int)json_object_dotget_number(root_object, "garcia.duration.being_thrown");
	standing_up_duration = (int)json_object_dotget_number(root_object, "garcia.duration.standing_up");
	
//----------------------- sprites ---------------------------
	j_array = json_object_dotget_array(root_object, "garcia.sprite_offset");
	sprite_offset.x = (int)json_array_get_number(j_array, 0);
	sprite_offset.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "garcia.sprite_offset_flip");
	sprite_offset_flip.x = (int)json_array_get_number(j_array, 0);
	sprite_offset_flip.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	idle.speed = (float)json_object_dotget_number(root_object, "garcia.idle.speed");
	j_array = json_object_dotget_array(root_object, "garcia.idle.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		idle.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	walk.speed = (float)json_object_dotget_number(root_object, "garcia.walk.speed");
	j_array = json_object_dotget_array(root_object, "garcia.walk.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		walk.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	attack1.speed = (float)json_object_dotget_number(root_object, "garcia.attack1.speed");
	j_array = json_object_dotget_array(root_object, "garcia.attack1.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		attack1.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	attack2 = attack1;

	attack3.speed = (float)json_object_dotget_number(root_object, "garcia.attack3.speed");
	j_array = json_object_dotget_array(root_object, "garcia.attack3.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		attack3.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	being_hit.speed = (float)json_object_dotget_number(root_object, "garcia.hit.speed");
	j_array = json_object_dotget_array(root_object, "garcia.hit.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		being_hit.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);



//--- free json 
	json_value_free(root_value);

	return true;
}
