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
	Entity(Types type);
	virtual ~Entity();

	virtual bool Init();
	virtual bool Update(unsigned int msec_elapsed, const bool upd_logic = false);
	virtual void IncreaseHealth(int amount);
	virtual void DecreaseHealth(int amount);

protected:
	virtual bool IsAlive();


	virtual bool AllowAnimationInterruption();
	virtual void UpdateCurrentAnimation(Animation& new_anim);

	virtual bool LoadFromConfigFile(const char* file_path);

public: 
	iPoint position = iPoint(0, 0);
	int	ground_y = 0;

	int lives = 3;
	int score = 0;
	int help = 3;

protected:
	Types m_type = Types::unknown;

//move variables
	iPoint speed = iPoint(0, 0);
	iPoint move_speed = iPoint(0, 0);
	bool grounded = true;

// jumping control
	int jump_remaining_cycles = 0;
	int jump_duration = 0;

//animation blocking	
	int blocking_animation_remaining_cycles = 0;
	int attacks_duration = 0;
	int take_item_duration = 0;
	int throwing_duration = 0;
	int holding_swap_duration = 0;
	int being_hold_attack_duration = 0;
	int being_thrown_duration = 0;
	int being_hit_duration = 0;
	int being_knocked_duration = 0;
	int standing_up_duration = 0;

// health & damage variables
	int health = 0;
	int max_health = 0;
	int attack1_dmg= 0;
	int attack2_dmg = 0;
	int attack3_dmg = 0;
	int attack_back_dmg = 0;
	int throw_dmg = 0;

// Colliders
	Collider *attack_collider = nullptr;
	Collider *hit_collider = nullptr;

// Graphics and animations
	SDL_Texture *graphics;
	Animation * current_animation = nullptr;
	iPoint sprite_offset = iPoint(0, 0);
	iPoint sprite_offset_flip = iPoint(0, 0);
	bool facing_right = true;

	Animation idle;
	Animation walk;
	Animation take_item;

	Animation attack1;
	Animation attack2;
	Animation attack3;
	Animation attack_back;

	Animation jump_prep;
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