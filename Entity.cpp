#include "Entity.h"

Entity::Entity(Types type) : m_type(type)
{}

Entity::~Entity()
{}

bool Entity::Init()
{
	if (LoadFromConfigFile(CONFIG_FILE) == false)
	{
		return false;
	}
	return true;

}

bool Entity::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	return true;
}

bool Entity::IsAlive()
{
	return health > 0;
}

bool Entity::AllowAnimationInterruption()
{
	return blocking_animation_remaining_cycles < 0;
}

void Entity::UpdateCurrentAnimation(Animation& new_anim)
{
	if (current_animation != &new_anim)
	{
		current_animation = &new_anim;
		current_animation->Reset();
	}

}



bool Entity::LoadFromConfigFile(const char* file_path) { 
	return true; 
}