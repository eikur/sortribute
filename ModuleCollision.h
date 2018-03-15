#pragma once

#include "Module.h"
#include "Point.h"
#include <functional>
#include <list>
#include <memory>

enum colliderType {
	PLAYER = 0,
	PLAYER_ATTACK,
	ITEMS,
	ENEMY,
	ENEMY_ATTACK,
	SCENE_TRIGGER
};

class Collider
{
	using CollisionCallback = std::function<void(colliderType)>;

	bool _toBeDeleted = false;
	colliderType _type;
	Entity* _parent = nullptr;
	bool _active = true;
	CollisionCallback _callback = nullptr;

  public: 
	  SDL_Rect rect = { 0,0,0,0 };

  public:
	Collider() {}
	Collider(SDL_Rect rectangle, colliderType t, Entity* parent = nullptr, const CollisionCallback& callback = nullptr) :
		rect(rectangle), _type(t), _parent(parent), _active(true), _callback(callback)
	{}

	colliderType getType() const;
	Entity* getParent() const;

	void setPos(iPoint pos);
	void SetRect(const SDL_Rect& r);
	const SDL_Rect& getRect() const;

	void setActive(bool value);
	bool isActive() const;

	bool hasToBeDeleted() const;
	void setToBeDeleted();

	bool hasCallback() const;
	void safeCallback(colliderType param);

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
	using CollisionCallback = std::function<void(colliderType)>;

public:

	ModuleCollision(Module& entitiesReporter, Module& sceneReporter);
	~ModuleCollision();

	UpdateStatus PreUpdate();
	UpdateStatus Update(float dt) override;

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect, colliderType type, Entity* parent = nullptr, const CollisionCallback& callback = nullptr);
	void DebugDraw();

private:
	std::list<std::unique_ptr<Collider>> _colliders;

	bool debug = false;
	int collision_matrix[6][6] = { { 0,0,1,1,1,1},{ 0,0,0,1,0,0 },{ 1,0,0,0,0,0 },{ 1,1,0,1,0,0 },{ 1,0,0,0,0,0 },{ 1,0,0,0,0,0 } };
	Module& entitiesReporter;
	Module& scenesReporter;
};
