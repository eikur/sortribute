#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager(bool active) : Module(active)
{}

SceneManager::~SceneManager() {}

bool SceneManager::Init()
{
	return true;
}
UpdateStatus SceneManager::PreUpdate()
{
	if(_currentScene)
		_currentScene->PreUpdate();
	return UpdateStatus::Continue;
}
UpdateStatus SceneManager::Update(float dt)
{
	if (_currentScene)
		_currentScene->Update(dt);
	return UpdateStatus::Continue;
}
UpdateStatus SceneManager::PostUpdate()
{
	if (_currentScene)
		_currentScene->PostUpdate();
	return UpdateStatus::Continue;
}
bool SceneManager::CleanUp()
{
	return true;
}