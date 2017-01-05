#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

#include "EnemyGarcia.h"

EnemyGarcia::EnemyGarcia(): Entity(Entity::Types::npc_garcia)
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
	if (IsAlive() == false)
	{
		return Die();
	}
	if (upd_logic)
	{
		if (blocking_animation_remaining_msec > 0)
			blocking_animation_remaining_msec -= msec_elapsed;
		
		// animation and status transition
		if (blocking_animation_remaining_msec <= 0)
		{
			if (current_animation == &being_hold_front_hit)
				UpdateCurrentAnimation(&being_hold_front);
			else if (current_animation == &being_thrown || current_animation == &being_knocked)
				UpdateCurrentAnimation(&standing_up, standing_up_duration);
			else if (current_animation == &standing_up)
				UpdateCurrentAnimation(&idle);
		}

		if (unhittable_remaining_msec > 0)
			unhittable_remaining_msec -= msec_elapsed;
		
		if (unhittable_remaining_msec <= 0 && (current_animation == &being_hit || current_animation == &being_hold_front_hit))
		{
			unhittable_remaining_msec = 0;
			is_hittable = true;
		}

		if (AllowAnimationInterruption())
		{
			if( is_being_hold_front == false && is_being_hold_back == false)
			{ 
				UpdateCurrentAnimation(&idle);
			}
			UpdatePosition({ 0,0 });
		}
	}

	return true;
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

// ------------------------ health ----------------------------
	max_health = (int)json_object_dotget_number(root_object, "garcia.max_health");
	health = max_health;
	lives = (int)json_object_dotget_number(root_object, "garcia.lives");
	score = (int)json_object_dotget_number(root_object, "garcia.score");

//----------------------- colliders ---------------------------
	j_array = json_object_dotget_array(root_object, "garcia.colliders.hit");
	hit_collider_offset = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	hit_collider = App->collision->AddCollider(
	{ hit_collider_offset.x + position.x, hit_collider_offset.y + position.y, (int)json_array_get_number(j_array, 2) , (int)json_array_get_number(j_array, 3) },
		colliderType::ENEMY, *this);

	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "garcia.colliders.attack");
	attack_collider_offset = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	attack_collider = App->collision->AddCollider(
	{ attack_collider_offset.x + position.x, attack_collider_offset.y + position.y, (int)json_array_get_number(j_array, 2) , (int)json_array_get_number(j_array, 3) },
		colliderType::ENEMY_ATTACK, *this);
	json_array_clear(j_array);

//----------------------- duration ---------------------------
	attacks_duration = (int)json_object_dotget_number(root_object, "garcia.duration.attacks");
	being_hit_duration = (int)json_object_dotget_number(root_object, "garcia.duration.being_hit");
	being_knocked_duration = (int)json_object_dotget_number(root_object, "garcia.duration.being_knocked");
	being_thrown_duration = (int)json_object_dotget_number(root_object, "garcia.duration.being_thrown");
	standing_up_duration = (int)json_object_dotget_number(root_object, "garcia.duration.standing_up");
	unhittable_max_msec = (int)json_object_dotget_number(root_object, "garcia.duration.unhittable");

	
//----------------------- sprites ---------------------------
	j_array = json_object_dotget_array(root_object, "garcia.sprite_offset");
	sprite_offset.x = (int)json_array_get_number(j_array, 0);
	sprite_offset.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "garcia.sprite_offset_flip");
	sprite_offset_flip.x = (int)json_array_get_number(j_array, 0);
	sprite_offset_flip.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);


	LoadAnimationFromJSONObject(root_object, "garcia.idle", &idle);
	LoadAnimationFromJSONObject(root_object, "garcia.walk", &walk);
	LoadAnimationFromJSONObject(root_object, "garcia.attack1", &attack1);
	LoadAnimationFromJSONObject(root_object, "garcia.attack3", &attack3);
	LoadAnimationFromJSONObject(root_object, "garcia.being_hit", &being_hit);
	LoadAnimationFromJSONObject(root_object, "garcia.being_hold_front", &being_hold_front);
	LoadAnimationFromJSONObject(root_object, "garcia.being_hold_front_hit", &being_hold_front_hit);
	LoadAnimationFromJSONObject(root_object, "garcia.being_hold_back", &being_hold_back);

	//--- free json 
	json_value_free(root_value);

	return true;
}
