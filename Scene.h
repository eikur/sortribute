#pragma once

#include "Globals.h"
#include "SceneManager.h"

struct Collider;

class Scene
{
public:
	Scene(SceneManager& manager);
	virtual ~Scene() = default;

	virtual bool Init();
	virtual bool Start();
	virtual UpdateStatus PreUpdate();
	virtual UpdateStatus Update(float dt = 0.0f);
	virtual UpdateStatus PostUpdate();
	virtual bool CleanUp();
	
	SceneManager& getManager() const;
	virtual void HandleCollision(Collider*, Collider*) {}

private:
	SceneManager& _manager;
};