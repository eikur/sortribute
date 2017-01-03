#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "parson.h"
#include "Point.h"
#include "Globals.h"
#include "Animation.h"

struct Collider;
struct SDL_Texture;
struct SDL_Rect;

class Entity 
{
public:
	enum Types
	{
		player,
		npc_garcia,
		unknown
	};
	struct ptrEntityDepthComparison {
		bool operator() (const Entity* left, const Entity* right) const {
			return left->GetDepth() < right->GetDepth();
		}
	};

public:
	Entity(Types type);
	virtual ~Entity();

	virtual bool Init();
	virtual bool Update(unsigned int msec_elapsed, const bool upd_logic = false);

	virtual void UpdatePosition(const iPoint new_speed);

	bool Draw() const;
	int GetDepth() const;

protected:
	bool IsAlive();
	void IncreaseHealth(int amount);
	void DecreaseHealth(int amount);

	bool AllowAnimationInterruption();
	void UpdateCurrentAnimation(Animation *new_anim,  int block_anim_duration = 0, int fx_new_anim = -1 );

	virtual bool LoadFromConfigFile(const char* file_path);

public: 
	iPoint position = {0, 0};
	int	ground_y = 0;

	int lives = 0;
	int score = 0;
	int help = 0;

protected:
	Types m_type = Types::unknown;

//move variables
	iPoint speed = {0, 0};
	iPoint move_speed = {0, 0};
	bool grounded = true;

// jumping control
	int air_remaining_msec = 0;
	int jump_prep_duration = 0;
	int jump_duration = 0;

//animation blocking	
	int blocking_animation_remaining_msec = 0;

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
	iPoint attack_collider_offset = {0, 0};
	iPoint hit_collider_offset = {0, 0};


// Graphics and animations
	SDL_Texture *graphics;
	Animation * current_animation = nullptr;
	iPoint sprite_offset = {0, 0};
	iPoint sprite_offset_flip = {0, 0};
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
	Animation jump_land;

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

	SDL_Rect shadow;

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