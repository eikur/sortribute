#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

class Player : public Entity {

public: 
	Player();
	~Player();

	bool Update();

	bool LoadFromConfigFile(const char* file_path);
};

#endif // __PLAYER_H__