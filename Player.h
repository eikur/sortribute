#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

class Player : public Entity {

public: 
	Player();
	~Player();

	bool Update( unsigned int msec_elapsed, const bool upd_logic = false);
	
private:
	void AddScore(int addition);
	void ModifyLives(int mod_to_add);

	bool LoadFromConfigFile(const char* file_path);
	void CheatCodes();

};

#endif // __PLAYER_H__