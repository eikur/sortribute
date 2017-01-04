#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
class EntityManager;

class Player : public Entity {

public: 
	Player();
	~Player();
	
	bool Init();
	bool Update( unsigned int msec_elapsed, const bool upd_logic = false);

	void AddScore(int addition);

private:
	void ModifyLives(int mod_to_add);
	void ReRaise();

	void UpdatePosition(const iPoint new_speed);

	void GetInput();
	void ResetInput();

	bool LoadFromConfigFile(const char* file_path);
	void CheatCodes();

private: 
	SDL_Rect position_limits = { 28, 162, 264, 55 };	// initial margins
	EntityManager *parent = nullptr;
	
	bool respawn_fall = true;

	int input_horizontal = 0; 
	int input_vertical = 0;
	bool input_help = false;
	bool input_attack = false;
	bool input_attack_back = false;
	bool input_jump = false;
	
};

#endif // __PLAYER_H__