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

void Entity::IncreaseHealth(int amount) {
	health = MIN(health + amount, max_health);
}
void Entity::DecreaseHealth(int amount) {
	health -= MAX(health - amount, 0);
}

bool Entity::IsAlive()
{
	return health > 0;
}

bool Entity::AllowAnimationInterruption()
{
	return blocking_animation_remaining_cycles < 0;
}

void Entity::UpdateCurrentAnimation(Animation *new_anim)
{
	if (current_animation != new_anim)
	{
		current_animation = new_anim;
		current_animation->Reset();
	}

}



bool Entity::LoadFromConfigFile(const char* file_path) { 
	return true; 
}