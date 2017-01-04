#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
class EntityManager;

class Player : public Entity {

public: 
	Player( EntityManager* parent);
	~Player();
	
	bool Init();
	bool Update( unsigned int msec_elapsed, const bool upd_logic = false);

	void AddScore(int addition);

private:
	void ModifyLives(int mod_to_add);
	void ReRaise();

	void UpdatePosition(const iPoint new_speed);

	bool LoadFromConfigFile(const char* file_path);
	void CheatCodes();

private: 
	SDL_Rect position_limits = { 28, 162, 264, 55 };	// initial margins
	EntityManager *parent = nullptr;
	
	bool respawn_fall = true;
};

#endif // __PLAYER_H__