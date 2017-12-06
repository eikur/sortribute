#pragma once

#include<list>
#include "Module.h"

enum colliderType {
	PLAYER = 0,
	PLAYER_ATTACK,
	ITEMS,
	ENEMY,
	ENEMY_ATTACK,
	SCENE_TRIGGER
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	colliderType type;
	Entity* parent = nullptr;

	Collider() {}
	Collider(SDL_Rect rectangle, colliderType t, Entity* parent = nullptr) :
		rect(rectangle), type(t), parent(parent)
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

	ModuleCollision(Module& entitiesReporter, Module& sceneReporter);
	~ModuleCollision();

	UpdateStatus PreUpdate();
	UpdateStatus Update(float dt) override;

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect, colliderType type, Entity* parent = nullptr);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
	int collision_matrix[6][6] = { { 0,0,1,1,1,1},{ 0,0,0,1,0,0 },{ 1,0,0,0,0,0 },{ 1,1,0,1,0,0 },{ 1,0,0,0,0,0 },{ 1,0,0,0,0,0 } };
	Module& entitiesReporter;
	Module& scenesReporter;
};
