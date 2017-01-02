#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"

enum colliderType {
	PLAYER = 0,
	PLAYER_ATTACK,
	ITEMS,
	ENEMY,
	ENEMY_ATTACK
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	colliderType type;

	Module* report_to;	// TODO: change the reporting method 


	Collider() {}
	Collider(SDL_Rect rectangle, colliderType t) : 
		rect(rectangle), type(t)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision(Module* report_to);
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect, colliderType type);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
	int collision_matrix[5][5] = { { 0,0,1,1,1 },{ 0,0,1,1,0 },{ 1,1,0,1,1 },{ 1,1,1,0,0 },{ 1,0,1,0,0 } };	// TODO: Load from file
	Module* report_to;
};

#endif // __ModuleCollision_H__