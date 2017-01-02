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

private:
	void AddScore(int addition);
	void ModifyLives(int mod_to_add);
	void ReRaise();
	void Die();

	void UpdatePosition(const iPoint speed);

	bool LoadFromConfigFile(const char* file_path);
	void CheatCodes();

private: 
	SDL_Rect position_limits = { 28, 162, 264, 55 };	// initial margins
	EntityManager *parent = nullptr;
};

#endif // __PLAYER_H__