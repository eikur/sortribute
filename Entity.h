#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "parson.h"
#include "Point.h"
#include "Globals.h"
class Animation;
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

	virtual bool Update()
	{
		return true; 
	}

	virtual bool IsDead()
	{
		return health <= 0;
	}

	virtual bool LoadFromConfigFile(const char* file_path) 
	{
		return true;
	}

protected:
	Types m_type = Types::unknown;
	iPoint position = iPoint(0, 0);
	int z_jump = 0;

// health & damage variables
	int health = 0;
	int attack1_dmg= 0;
	int attack2_dmg = 0;
	int attack3_dmg = 0;
	int attack_back_dmg = 0;
	int front_throw_dmg = 0;
	int back_throw_dmg = 0;

// Graphics and animations
	SDL_Texture *graphics;

	Animation *idle = nullptr;
	Animation *walk = nullptr;
	Animation *take_item = nullptr;

	Animation *attack1 = nullptr;
	Animation *attack2 = nullptr;
	Animation *attack3 = nullptr;
	Animation *attack_back = nullptr;

	Animation *jump = nullptr;
	Animation *jump_attack = nullptr;

	Animation *holding_front = nullptr;
	Animation *holding_front_attack = nullptr;
	Animation *holding_front_attack2 = nullptr;
	Animation *holding_back = nullptr;
	Animation *throwing_front = nullptr;
	Animation *throwing_back = nullptr;
	Animation *holding_swap = nullptr;
	
	Animation *being_hold = nullptr;
	Animation *being_hold_attack = nullptr;
	Animation *being_thrown = nullptr;
	Animation *being_hit = nullptr;
	Animation *being_knocked = nullptr;
	Animation *standing_up = nullptr;
// sounds
	unsigned int fx_voice = 0;
	unsigned int fx_jump = 0;
	unsigned int fx_landing_jump = 0;
	unsigned int fx_landing_knocked = 0;
	unsigned int fx_attack_miss = 0;
	unsigned int fx_attack_hit = 0;
	unsigned int fx_death = 0;


	
	



};

#endif // __ENTITY_H__