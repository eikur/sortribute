#include "SceneManager.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "Stage3.h"

SceneManager::SceneManager(bool active) : Module(active)
{
	_intro = std::make_unique<SceneIntro>(*this);
	_stage3 = std::make_unique<Stage3>(*this);
}

SceneManager::~SceneManager() {}

bool SceneManager::Init()
{
	getIntro().Init();
	getStage3().Init();
	SwapScene(SceneId::Intro);
	return true;
}
UpdateStatus SceneManager::PreUpdate()
{
	_currentScene->PreUpdate();
	return UpdateStatus::Continue;
}
UpdateStatus SceneManager::Update(float dt)
{
	_currentScene->Update(dt);
	return UpdateStatus::Continue;
}
UpdateStatus SceneManager::PostUpdate()
{
	_currentScene->PostUpdate();
	return UpdateStatus::Continue;
}
bool SceneManager::CleanUp()
{
	return true;
}


void SceneManager::SwapScene(SceneId sceneId)
{
	switch (sceneId)
	{
	case SceneId::Intro:
		_currentScene = &getIntro();
		break;
	case SceneId::Stage3: 
		_currentScene = &getStage3();
		break;
	case SceneId::CharacterSelect:
	case SceneId::Highscores:
	case SceneId::MainMenu:
	case SceneId::Stage4:
	case SceneId::Stage5:
	case SceneId::Stage6:
	case SceneId::Stage7:
	case SceneId::Stage8:
		break;
	}
	if (_currentScene)
	{
		_currentScene->Start();
	}
}

void SceneManager::HandleCollision(Collider* first, Collider* second)
{
 	_currentScene->HandleCollision( first, second);
}