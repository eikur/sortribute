#pragma once

#include "Globals.h"
#include "SceneManager.h"
#include "EntityManager.h"

struct Collider;

class Scene
{
public:
	Scene(SceneManager& manager, EntityManager& entityManager);

	virtual bool Init();
	virtual bool Start();
	virtual UpdateStatus PreUpdate();
	virtual UpdateStatus Update(float dt = 0.0f);
	virtual UpdateStatus PostUpdate();
	virtual bool CleanUp();
	
	virtual void HandleCollision(Collider*, Collider*) {}

	SceneManager& getManager();
	EntityManager& getEntityManager();
private:
	SceneManager& _manager;
	EntityManager& _entityManager;

	const float totalRemainingTimeMsec = 81999.0f;
	const float remainingTimeMsec = 0;
};