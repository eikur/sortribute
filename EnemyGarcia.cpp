#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "EntityManager.h"

#include "EnemyGarcia.h"

EnemyGarcia::EnemyGarcia( Entity* target): Entity(Entity::Types::npc_garcia), target(target)
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
	facing_right = false;
	state = none;
	return true;
}

bool EnemyGarcia::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	if (blocking_animation_remaining_msec > 0)
		blocking_animation_remaining_msec = MAX(blocking_animation_remaining_msec - msec_elapsed, 0);
	if (air_remaining_msec > 0)
		air_remaining_msec = MAX(air_remaining_msec - msec_elapsed, 0);
	if (unhittable_remaining_msec > 0)
		unhittable_remaining_msec = MAX(unhittable_remaining_msec - msec_elapsed, 0);

	if (IsAlive() == false)
	{
		RemoveColliders();
		if (upd_logic && current_animation == &being_knocked)
			UpdatePosition(UpdateKnockedMotion());
		if (blocking_animation_remaining_msec <= 0 && current_animation != &dying)
			UpdateCurrentAnimation(&dying, dying_duration);
		if (blocking_animation_remaining_msec <= 0 && current_animation == &dying)
		{
			CleanUp();
			return false;
		}
		return true;
	}

	move_speed = { 0,0 };
	if (grounded == false)
	{
		if (air_remaining_msec > 0)
		{
			if (is_being_thrown_front)
				move_speed = UpdateThrownFrontMotion();
			else
				move_speed = UpdateKnockedMotion();
		}
		else
		{
			position.y = ground_y;
		}
	}

	if(is_being_thrown_back)
		UpdateThrownBackMotion();

	if (blocking_animation_remaining_msec <= 0)
	{
		if (current_animation == &being_hold_front_hit)
			UpdateCurrentAnimation(&being_hold_front);
		else if (current_animation == &being_thrown_front || current_animation == &being_thrown_back)
		{
			DecreaseHealth(throw_dmg);
			if (IsAlive())
			{
				UpdateCurrentAnimation(&standing_up, standing_up_duration);
				state = none;
			}
		}
		else if (current_animation == &being_knocked)
		{
			UpdateCurrentAnimation(&standing_up, standing_up_duration, fx_ground_hit);
			state = none;
		}
		else if (current_animation == &standing_up)
			UpdateCurrentAnimation(&idle);
		else if (current_animation == &being_hit)
			UpdateCurrentAnimation(&idle);
		else if (current_animation == &attack1 || current_animation == &attack2)
			UpdateCurrentAnimation(&idle, attack_pause);
	}

	if (unhittable_remaining_msec <= 0 && (current_animation == &being_hit || current_animation == &being_hold_front_hit))
		is_hittable = true;

	// IA start
	if (AllowAnimationInterruption() && is_being_hold_front == false && is_being_hold_back == false)
	{
		int decision = rand() % 101;
		int attack_range = attack_collider->rect.w + attack_collider_offset.x;
			
		if (InEnemyActionQueue() )		
		{ 
			switch (state) {
			case none:
				if (target->IsAlive())
				{
					if (decision >= 50)	// direct 
						state = frontal_attack;
					else if (decision >= 20)
						state = taunting;
					else
					{
						state = switching_sides;
						// destination point needed
					}
				}
				break;
			case frontal_attack:
				facing_right = target->position.x > position.x;
				if (abs(target->position.x - position.x) <= attack_range && abs(target->GetDepth() - GetDepth()) <= 5)
				{
					if (current_combo_hits <= 1)
						UpdateCurrentAnimation(&attack1, attacks_duration);
					else
						UpdateCurrentAnimation(&attack2, attacks_duration);
					current_combo_hits += 1;
				}
				else
				{
					move_speed = SpeedTowardsTarget();
					UpdateCurrentAnimation(&walk);
				}
				break;
			}
		}
		else
		{
			state = none;
		}
	}
	
	if (upd_logic)
		UpdatePosition(move_speed);

	return true;
}

void EnemyGarcia::CleanUp()
{
	App->manager->enemy_queue.remove(this);
}

bool EnemyGarcia::InEnemyActionQueue() const
{
	unsigned int max_enemies_queue = 2;	
	unsigned int enemies_in_queue = App->manager->enemy_queue.size();

	if (enemies_in_queue == 0)
	{
		App->manager->enemy_queue.push_back((Entity*) this);
		return true;
	}

	for (std::list<Entity*>::const_iterator it = App->manager->enemy_queue.cbegin(); it != App->manager->enemy_queue.cend(); ++it)
		if (*it == this)
			return true;
	
	if (enemies_in_queue < max_enemies_queue) {
		App->manager->enemy_queue.push_back((Entity*) this);
		return true;
	}
	else
		return false;	
}

iPoint EnemyGarcia::SpeedTowardsTarget() const
{
	iPoint ret = { 0,0 };
	ret.x = (target->position.x - position.x) / 10;
	ret.y = (target->position.y - position.y) / 10;
	
	return ret;
}
bool EnemyGarcia::LoadFromConfigFile(const char* file_path) 
{
	JSON_Value *root_value;
	JSON_Object *root_object;

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

// -------------------- damages -------------------------
	attack1_dmg = (int)json_object_dotget_number(root_object, "damages.attack1");
	attack2_dmg = (int)json_object_dotget_number(root_object, "damages.attack2");
	throw_dmg = (int)json_object_dotget_number(root_object, "damages.throw");

//----------------------- colliders ---------------------------
	hit_collider = LoadColliderFromJSONObject(root_object, "garcia.colliders.hit", colliderType::ENEMY, &hit_collider_offset);
	attack_collider = LoadColliderFromJSONObject(root_object, "garcia.colliders.attack", colliderType::ENEMY_ATTACK, &attack_collider_offset);

//----------------------- duration ---------------------------
	attacks_duration = (int)json_object_dotget_number(root_object, "durations.attacks");
	being_hit_duration = (int)json_object_dotget_number(root_object, "durations.being_hit_enemy");
	being_knocked_duration = (int)json_object_dotget_number(root_object, "durations.being_knocked");
	being_thrown_duration = (int)json_object_dotget_number(root_object, "durations.being_thrown");
	standing_up_duration = (int)json_object_dotget_number(root_object, "durations.standing_up_enemy");
	unhittable_max_msec = (int)json_object_dotget_number(root_object, "durations.unhittable");
	dying_duration = (int)json_object_dotget_number(root_object, "durations.dying");
	attack_pause = (int)json_object_dotget_number(root_object, "durations.attack_pause");
	
//----------------------- sprites ---------------------------
	LoadiPointFromJSONObject(root_object, "garcia.sprite_offset", &sprite_offset);
	LoadiPointFromJSONObject(root_object, "garcia.sprite_offset_flip", &sprite_offset_flip);

//----------------------- animations ---------------------------
	LoadAnimationFromJSONObject(root_object, "garcia.idle", &idle);
	LoadAnimationFromJSONObject(root_object, "garcia.walk", &walk);
	LoadAnimationFromJSONObject(root_object, "garcia.attack1", &attack1);
	LoadAnimationFromJSONObject(root_object, "garcia.attack2", &attack2);
	LoadAnimationFromJSONObject(root_object, "garcia.being_hit", &being_hit);
	LoadAnimationFromJSONObject(root_object, "garcia.being_hold_front", &being_hold_front);
	LoadAnimationFromJSONObject(root_object, "garcia.being_hold_front_hit", &being_hold_front_hit);
	LoadAnimationFromJSONObject(root_object, "garcia.being_hold_back", &being_hold_back);
	LoadAnimationFromJSONObject(root_object, "garcia.being_knocked", &being_knocked);
	LoadAnimationFromJSONObject(root_object, "garcia.being_thrown_front", &being_thrown_front);
	LoadAnimationFromJSONObject(root_object, "garcia.being_thrown_back", &being_thrown_back);
	LoadAnimationFromJSONObject(root_object, "garcia.standing_up", &standing_up);
	LoadAnimationFromJSONObject(root_object, "garcia.dying", &dying);
	LoadSDLRectFromJSONObject(root_object, "garcia.shadow", &shadow);

//----------------------- sounds ---------------------------
	LoadSoundFXFromJSONObject(root_object, "fx.death_garcia", &fx_death);
	LoadSoundFXFromJSONObject(root_object, "fx.attack_hit_enemy", &fx_attack_hit);
	LoadSoundFXFromJSONObject(root_object, "fx.ground_hit", &fx_ground_hit);

	//--- free json 
	json_value_free(root_value);

	return true;
}
