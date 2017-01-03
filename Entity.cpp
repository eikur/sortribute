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

//------------------- Updates ---------------------
bool Entity::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	return true;
}

void Entity::UpdatePosition(const iPoint new_speed)
{
	if (grounded)
		ground_y = position.y;

	position += new_speed;

	//apply offset to colliders
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
	
}

//------------------- Draw to screen ---------------------
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

//----------------------------------------------------

bool Entity::IsAlive()
{
	return health > 0;
}

void Entity::IncreaseHealth(int amount) {
	health = MIN(health + amount, max_health);
}
void Entity::DecreaseHealth(int amount) {
	health = MAX(health - amount, 0);
}

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

		// update the hittable status!
		if (current_animation == &being_hit || 
			current_animation == &being_thrown ||
			current_animation == &being_knocked ||
			current_animation == &throwing_front ||
			current_animation == &throwing_back ||
			current_animation == &standing_up ||
			current_animation == &jump_attack)
			hittable = false;
		else
			hittable = true;

		// update the attacking status
		if (current_animation == &attack1 ||
			current_animation == &attack2 ||
			current_animation == &attack3 ||
			current_animation == &jump_attack)
			attacking = true;
		else
			attacking = false;
	}
}



bool Entity::LoadFromConfigFile(const char* file_path) { 
	return true; 
}