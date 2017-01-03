#include "Application.h"
#include "ModuleRender.h"
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

void Entity::UpdatePosition(const iPoint speed)
{}

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
	health -= MAX(health - amount, 0);
}

bool Entity::AllowAnimationInterruption()
{
	return blocking_animation_remaining_msec <= 0;
}

void Entity::UpdateCurrentAnimation(Animation *new_anim, int block_anim_duration)
{
	if (current_animation != new_anim)
	{
		current_animation = new_anim;
		current_animation->Reset();
	}
	blocking_animation_remaining_msec = block_anim_duration;

}



bool Entity::LoadFromConfigFile(const char* file_path) { 
	return true; 
}