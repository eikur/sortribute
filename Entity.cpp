#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

#include "Entity.h"

Entity::Entity(Types type) : m_type(type)
{}

Entity::~Entity()
{}

bool Entity::Init()
{
	return true;
}

bool Entity::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	return true;
}

void Entity::UpdatePosition(const iPoint new_speed)
{
	if (grounded)
		ground_y = position.y;

	position += new_speed;

	if (facing_right)
	{
		attack_collider->rect.x = position.x + attack_collider_offset.x;
		hit_collider->rect.x = position.x + hit_collider_offset.x;
	}
	else
	{
		attack_collider->rect.x = position.x + -(attack_collider_offset.x + attack_collider->rect.w);
		hit_collider->rect.x = position.x - (hit_collider_offset.x + hit_collider->rect.w);
	}
	attack_collider->rect.y = position.y + attack_collider_offset.y;
	hit_collider->rect.y = position.y + hit_collider_offset.y;
	if (grounded)
		ground_y = position.y;
	
}


bool Entity::Draw() const
{
	if (facing_right)
	{
		App->renderer->Blit(graphics, position.x + sprite_offset.x, position.y + sprite_offset.y, &(current_animation->GetCurrentFrame()), 1.0F, false);
		if (grounded == false)
			App->renderer->Blit(graphics, position.x + sprite_offset.x, ground_y + sprite_offset.y, &shadow, 1.0f, false);
	}
	else
	{
		App->renderer->Blit(graphics, position.x + sprite_offset_flip.x, position.y + sprite_offset_flip.y, &(current_animation->GetCurrentFrame()), 1.0F, true);
		if (grounded == false)
			App->renderer->Blit(graphics, position.x + sprite_offset_flip.x, ground_y + sprite_offset_flip.y, &shadow, 1.0f, true);
	}
	return true;
}

int Entity::GetDepth() const {
	return ground_y;
}

void Entity::SetDepth(int new_depth) {
	int diff = new_depth - ground_y;
	UpdatePosition({ 0,diff });
}

bool Entity::IsGrounded() const 
{
	return grounded;
}

//----------------------------------------------------
bool Entity::IsAlive() const
{
	return health > 0;
}

void Entity::IncreaseHealth(int amount) {
	health = MIN(health + amount, max_health);
}

void Entity::DecreaseHealth(int amount) {
	health = MAX(health - amount, 0);
}
bool Entity::Die()
{
	if (hit_collider != nullptr)
		hit_collider->to_delete = true;
	if (attack_collider != nullptr)
		attack_collider->to_delete = true;
	return false;
}

void Entity::AddScore(int amount) 
{}

//-------------------    Setters for collision management---------------------------------
void Entity::SetIdle()
{
	UpdateCurrentAnimation(&idle);
}

void Entity::SetBeingHit(int damage){
	DecreaseHealth(damage);
	UpdateCurrentAnimation(&being_hit, being_hit_duration);
}

void Entity::SetHoldingFront(Entity* held)
{
	held_entity = held;
	UpdateCurrentAnimation(&holding_front);
}

void Entity::SetHoldingBack(Entity* held)
{
	held_entity = held;
	UpdateCurrentAnimation(&holding_back);
}

void Entity::SetBeingHoldFront()
{
	UpdateCurrentAnimation(&being_hold_front);
}

void Entity::SetBeingHoldBack()
{
	UpdateCurrentAnimation(&being_hold_back);
}

//----------------------------------------------------
bool Entity::AllowAnimationInterruption()
{
	return blocking_animation_remaining_msec <= 0;
}

void Entity::UpdateCurrentAnimation(Animation *new_anim,  int block_anim_duration, int fx_new_anim )
{
	if (current_animation != new_anim)
	{
		current_animation = new_anim;
		current_animation->Reset();
		if (fx_new_anim != -1)
			App->audio->PlayFx(fx_new_anim);
		blocking_animation_remaining_msec = block_anim_duration;

	// grounded update
		if (current_animation == &jump ||
			current_animation == &jump_attack ||
			current_animation == &being_thrown ||
			current_animation == &being_knocked 
			)
			grounded = false;
		else
			grounded = true;
	
	//update jumping status
		if (current_animation == &jump || current_animation == &jump_attack)
			jumping = true;
		else
			jumping = false;

		// update the hittable status!
		if (current_animation == &being_hit || 
			current_animation == &being_thrown ||
			current_animation == &being_knocked ||
			current_animation == &throwing_front ||
			current_animation == &throwing_back ||
			current_animation == &standing_up ||
			current_animation == &jump_attack)
			is_hittable = false;
		else
			is_hittable = true;

		// update the attacking status
		if (current_animation == &attack1 ||
			current_animation == &attack2 ||
			current_animation == &attack3 ||
			current_animation == &jump_attack)
			is_attacking = true;
		else
			is_attacking = false;
		
		// holding status
		if (current_animation == &holding_front || current_animation == &holding_back || current_animation == &holding_front_attack || current_animation == &holding_front_attack2)
			is_holding = true;
		else
			is_holding = false;

		if (current_animation == &being_hold_back || current_animation == &being_hold_front)
			is_being_hold = true;
		else
			is_being_hold = false;
	}
}

//----------------------------------------------------
bool Entity::LoadFromConfigFile(const char* file_path) { 
	return true; 
}