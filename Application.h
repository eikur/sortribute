#pragma once

#include "Globals.h"
#include "Module.h"
#include <memory>
#include <vector>

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleParticles;
class ModuleFonts;
class ModuleUI;
class ModuleSceneIntro;
class ModuleScene3;
class EntityManager;
class Timer;
class ConfigurationLoader;
class SceneManager;

// Defines a unique ptr of a class member, and its getter
#define MEMBER_DECL( _CLASS_, _NAME_, _GETTER_ )	\
		private:								\
			std::unique_ptr<_CLASS_> _NAME_;	\
		public:									\
			_CLASS_& _GETTER_() const	\
			{							\
				return *_NAME_.get();	\
			}
				
class Application
{
public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

private:
	std::vector<Module*> _modules;

	MEMBER_DECL(ModuleRender, _renderer, getRenderer)
	MEMBER_DECL(ModuleWindow, _window, getWindow)
	MEMBER_DECL(ModuleTextures, _textures, getTextures)
	MEMBER_DECL(ModuleInput, _input, getInput)
	MEMBER_DECL(ModuleAudio, _audio, getAudio)
	MEMBER_DECL(ModuleFonts, _fonts, getFonts)
	MEMBER_DECL(ModuleUI, _ui, getUI)
	MEMBER_DECL(ModuleFadeToBlack, _fade, getFade)
	MEMBER_DECL(ModuleSceneIntro, _intro, getIntro)
	MEMBER_DECL(ModuleScene3, _scene3, getScene3)
	MEMBER_DECL(EntityManager, _entityManager, getEntityManager)
	MEMBER_DECL(ModuleCollision, _collision, getCollision)
	MEMBER_DECL(ModuleParticles, _particles, getParticles)
	MEMBER_DECL(SceneManager, _sceneManager, getSceneManager)

	MEMBER_DECL(ConfigurationLoader, _config, getConfig)
	MEMBER_DECL(Timer, _timer, getTimer)
};

extern Application* App;
