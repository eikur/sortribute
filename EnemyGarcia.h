#ifndef __ENEMYGARCIA_H__
#define __ENEMYGARCIA_H__

#include "Entity.h"
class EntityManager;

class EnemyGarcia: public Entity {

public:
	EnemyGarcia(EntityManager* parent);
	~EnemyGarcia();

	bool Init();
	bool Update(unsigned int msec_elapsed, const bool upd_logic = false);

private:
	void Die();
	bool LoadFromConfigFile(const char* file_path);
	
private: 
	EntityManager* parent = nullptr;

};

#endif // __ENEMYGARCIA_H__