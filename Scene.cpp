#include "Scene.h"

Scene::Scene(SceneManager& manager) : _manager(manager)
{}

bool Scene::Init()
{
	return true;
}

bool Scene::Start()
{
	return true;
}

UpdateStatus Scene::PreUpdate() 
{
	return UpdateStatus::Continue;
}

UpdateStatus Scene::Update(float)
{
	return UpdateStatus::Continue;
}

UpdateStatus Scene::PostUpdate()
{
	return UpdateStatus::Continue;
}
bool Scene::CleanUp()
{
	return true;
}

SceneManager& Scene::getManager() const
{
	return _manager;
}