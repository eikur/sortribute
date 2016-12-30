#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

class Player : public Entity {

public: 
	Player();
	~Player();

	bool Update( const bool upd_logic);
	
private:
	void AddScore(int addition);
	void ModifyLives(int mod_to_add);

	bool LoadFromConfigFile(const char* file_path);
	void CheatCodes();

private:
	int score = 0;
	int lives = 1;
	int help = 1;
	iPoint hud_score_pos = iPoint(0, 0);
	iPoint hud_help_pos = iPoint(0, 0);
	iPoint hud_lives_pos = iPoint(0, 0);

};

#endif // __PLAYER_H__