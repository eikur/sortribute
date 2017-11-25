#pragma once

#include "Module.h"

class Scene;

class SceneManager : public Module
{
public:
	enum class SceneId
	{
		CharacterSelect,
		Highscores,
		Intro,
		MainMenu,
		Stage3,
		Stage4,
		Stage5,
		Stage6,
		Stage7,
		Stage8
	};

public:
	SceneManager(bool active = true);
	~SceneManager();

	bool Init() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update(float) override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;

private:
	Scene* _currentScene = nullptr;
};