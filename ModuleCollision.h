#pragma once

#include <list>
#include <memory>
#include "Module.h"
#include "Point.h"

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
	bool _active = true;

	Collider() {}
	Collider(SDL_Rect rectangle, colliderType t, Entity* parent = nullptr) :
		rect(rectangle), type(t), parent(parent), _active(true)
	{}

	void setPos(iPoint pos)
	{
		rect.x = pos.x;
		rect.y = pos.y;
	}
	void SetRect(const SDL_Rect& r)
	{
		rect.x = r.x;
		rect.y = r.y; 
		rect.h = r.h;
		rect.w = r.w;
	}
	const SDL_Rect& getRect() const
	{
		return rect;
	}

	bool CheckCollision(const SDL_Rect& r) const;

	void setActive(bool value)
	{
		_active = value;
	}
	bool isActive() const
	{
		return _active;
	}
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
	std::list<std::unique_ptr<Collider>> _colliders;

	bool debug = false;
	int collision_matrix[6][6] = { { 0,0,1,1,1,1},{ 0,0,0,1,0,0 },{ 1,0,0,0,0,0 },{ 1,1,0,1,0,0 },{ 1,0,0,0,0,0 },{ 1,0,0,0,0,0 } };
	Module& entitiesReporter;
	Module& scenesReporter;
};
