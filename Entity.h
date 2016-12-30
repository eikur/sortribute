#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "parson.h"
#include "Point.h"
#include "Globals.h"
#include "Animation.h"

struct Collider;
struct SDL_Texture;

class Entity 
{
public:
	enum Types
	{
		player,
		unknown
	};


public:
	Entity(Types type) : m_type(type)
	{}

	virtual ~Entity()
	{}

	virtual bool Init()
	{
		if (LoadFromConfigFile(CONFIG_FILE) == false)
		{
			return false;
		}
		return true;

	}

	virtual bool Update( const bool upd_logic = false)
	{
		return true;
	}

	virtual bool IsAlive()
	{
		return health > 0;
	}

	virtual bool LoadFromConfigFile(const char* file_path) 
	{
		return true;
	}

protected:
	Types m_type = Types::unknown;

//move variables
	iPoint position = iPoint(0, 0);
	iPoint speed = iPoint(0, 0);
	int	ground_y = 0;
	bool grounded = true;


// health & damage variables
	int health = 0;
	int attack1_dmg= 0;
	int attack2_dmg = 0;
	int attack3_dmg = 0;
	int attack_back_dmg = 0;
	int front_throw_dmg = 0;
	int back_throw_dmg = 0;

// Colliders
	Collider *attack_collider = nullptr;
	Collider *hit_collider = nullptr;

// Graphics and animations
	SDL_Texture *graphics;
	Animation *current_animation = nullptr;
	iPoint sprite_offset = iPoint(0, 0);
	bool facing_right = true;

	Animation idle;
	Animation walk;
	Animation take_item;

	Animation attack1;
	Animation attack2;
	Animation attack3;
	Animation attack_back;

	Animation jump;
	Animation jump_attack;

	Animation holding_front;
	Animation holding_front_attack;
	Animation holding_front_attack2;
	Animation holding_back;
	Animation throwing_front;
	Animation throwing_back;
	Animation holding_swap;
	
	Animation being_hold;
	Animation being_hold_attack;
	Animation being_thrown;
	Animation being_hit;
	Animation being_knocked;
	Animation standing_up;
// sounds
	unsigned int fx_voice = 0;
	unsigned int fx_jump = 0;
	unsigned int fx_landing_jump = 0;
	unsigned int fx_landing_knocked = 0;
	unsigned int fx_attack_miss = 0;
	unsigned int fx_attack_hit = 0;
	unsigned int fx_death = 0;
	unsigned int fx_health_restore = 0;
	unsigned int fx_extra_life = 0;
};

#endif // __ENTITY_H__