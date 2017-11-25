#pragma once

#include "Globals.h"
#include "SceneManager.h"

class Scene
{
public:
	class Scene(SceneManager& manager);
	UpdateStatus PreUpdate();
	UpdateStatus Update(float dt = 0.0f);
	UpdateStatus PostUpdate();
	bool CleanUp();

private:
	SceneManager& _manager;
};