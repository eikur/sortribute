#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"

class Apple : public Entity {

public: 
	Apple();
	~Apple();

	bool Init();
	bool Update(unsigned int msec_elapsed, const bool upd_logic);
	void DecreaseHealth(int amount);

private:
	bool LoadFromConfigFile(const char* file_path);
};

#endif // __ITEM_H__
