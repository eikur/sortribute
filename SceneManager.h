#pragma once

#include "Module.h"
#include <memory>

class Stage3;
class Scene;
class SceneIntro;

// Defines a unique ptr of a class member, and its getter
#define SCENE_DECL( _CLASS_, _NAME_, _GETTER_ )	\
		private:								\
			std::unique_ptr<_CLASS_> _NAME_;	\
		public:									\
			_CLASS_& _GETTER_() const	\
			{							\
				return *_NAME_.get();	\
			}

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
	bool Start() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update(float) override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;

	void SwapScene(SceneId sceneId, float duration = 4.0f );
	void HandleCollision(Collider*, Collider*) override;

private:
	bool IsInTransition() const;
	void UpdateTransition(float dt);

private:
	Scene* _currentScene = nullptr;
	Scene* _nextScene = nullptr;

	SCENE_DECL(Stage3, _stage3, getStage3)
	SCENE_DECL(SceneIntro, _intro, getIntro)

	bool _inTransition = false;
	float _transitionTime = 0.0f;
	float _transitionDuration = 0.0f;

};