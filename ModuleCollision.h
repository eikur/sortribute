#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"

// TODO 9: Create a matrix of game specific types of collision for early discard
// Example: lasers should not collide with lasers but should collider with walls
// enemy shots will collide with other enemies ? and against walls ?

enum colliderType {
	PLAYER = 0,
	PLAYER_SHOT,
	WALLS,
	ENEMY,
	ENEMY_SHOT
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	colliderType type;

	// TODO 10: Add a way to notify other classes that a collision happened
	Module* report_to;


	Collider() {}
	Collider(SDL_Rect rectangle, colliderType t) : // expand this call if you need to
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
	int collision_matrix[5][5] = { { 0,0,1,1,1 },{ 0,0,1,1,0 },{ 1,1,0,1,1 },{ 1,1,1,0,0 },{ 1,0,1,0,0 } };
	Module* report_to;
};

#endif // __ModuleCollision_H__