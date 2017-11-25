#include "Application.h"
#include "ModuleRender.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "Stage3.h"
#include <algorithm>

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

bool SceneManager::Start()
{
	SDL_SetRenderDrawBlendMode(App->getRenderer().renderer, SDL_BLENDMODE_BLEND);
	return true;
}

UpdateStatus SceneManager::PreUpdate()
{
	if (_currentScene)
	{
		_currentScene->PreUpdate();
	}
	return UpdateStatus::Continue;
}
UpdateStatus SceneManager::Update(float dt)
{
	if (_currentScene)
	{
		_currentScene->Update(dt);
	}
	if (IsInTransition())
	{
		UpdateTransition(dt);
	}
	return UpdateStatus::Continue;
}
UpdateStatus SceneManager::PostUpdate()
{
	if (_currentScene)
	{
		_currentScene->PostUpdate();
	}
	return UpdateStatus::Continue;
}
bool SceneManager::CleanUp()
{
	return true;
}


void SceneManager::SwapScene(SceneId sceneId, float duration)
{
	switch (sceneId)
	{
	case SceneId::Intro:
		_nextScene = &getIntro();
		break;
	case SceneId::Stage3: 
		_nextScene = &getStage3();
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
	_transitionDuration = duration * 0.5f * 1000.0f;
	_transitionTime = _transitionDuration;
	_inTransition = true;
}

void SceneManager::HandleCollision(Collider* first, Collider* second)
{
 	_currentScene->HandleCollision( first, second);
}

bool SceneManager::IsInTransition() const
{
	return _inTransition;
}

void SceneManager::UpdateTransition(float dt)
{
	LOG("I AM IN TRANSITION****\n\n");
	_transitionTime -= dt;

	if (_currentScene == nullptr && _nextScene != nullptr)
	{
		_currentScene = _nextScene;
		_nextScene = nullptr;
		_currentScene->Start();
	}

	float opacity = std::max(_transitionTime / _transitionDuration, 0.0f);
	if (_nextScene != nullptr)
	{
		opacity = 1.0f - opacity;
	}

	SDL_SetRenderDrawColor(App->getRenderer().renderer, 0, 0, 0, static_cast<Uint8>(opacity * 255.0f));
	SDL_RenderFillRect(App->getRenderer().renderer, NULL);

	if (_transitionTime <= 0.0f)
	{
		if (_nextScene != nullptr)
		{
			_transitionTime = _transitionDuration;
			_currentScene = _nextScene;
			_currentScene->Start();
			_nextScene = nullptr;
		}
		else
		{
			_inTransition = false;
		}
	}
}