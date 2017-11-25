#include "Scene.h"

Scene::Scene(SceneManager& manager) : _manager(manager)
{}

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