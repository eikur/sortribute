#include "Application.h"
#include "ConfigurationLoader.h"
#include "EntityManager.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleScene3.h"
#include "ModuleTextures.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "SceneManager.h"
#include "Timer.h"

Application::Application()
{
	_config = std::make_unique<ConfigurationLoader>(CONFIG_FILE);
	_timer = std::make_unique<Timer>();
	_timer->TimerStart();

	// Base App modules
	_window = std::make_unique<ModuleWindow>();
	_renderer = std::make_unique<ModuleRender>();
	_input = std::make_unique<ModuleInput>();
	_textures = std::make_unique<ModuleTextures>();
	_audio = std::make_unique<ModuleAudio>();
	_fonts = std::make_unique<ModuleFonts>();
	_ui = std::make_unique<ModuleUI>(false);
	//Specific game modules
	_entityManager = std::make_unique<EntityManager>(false);
	_scene3 = std::make_unique<ModuleScene3>(false);
	_intro = std::make_unique<ModuleSceneIntro>(false);
	// Modules on top of game logic
	_collision = std::make_unique<ModuleCollision>((Module*)&getEntityManager(), (Module*)&getScene3());
	_particles = std::make_unique<ModuleParticles>();
	_fade = std::make_unique<ModuleFadeToBlack>();

	_sceneManager = std::make_unique<SceneManager>();

	// Order matters: they will init/start/pre/update/post in this order
	_modules = { _window.get(), _renderer.get(), _input.get(), _textures.get(), _audio.get(), _fonts.get(), _ui.get(), _entityManager.get(), _scene3.get(), _intro.get(), _collision.get(), _particles.get(), _fade.get(), _sceneManager.get()};
}

Application::~Application()
{
	_sceneManager.reset();
	_fade.reset();
	_particles.reset();
	_collision.reset();
	_intro.reset();
	_scene3.reset();
	_entityManager.reset();
	_ui.reset();
	_fonts.reset();
	_audio.reset();
	_textures.reset();
	_input.reset();
	_renderer.reset();
	_window.reset();

	_timer.reset();
	_config.reset();
}

bool Application::Init()
{
	bool ret = true;

	for(auto it = _modules.begin(); it != _modules.end() && ret; ++it)
		ret = (*it)->Init();

	for(auto it = _modules.begin(); it != _modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	getFade().FadeToBlack(&getIntro(), nullptr);

	return ret;
}

UpdateStatus Application::Update()
{
	UpdateStatus ret = UpdateStatus::Continue;

	getTimer().UpdateDeltaTime();

	for(auto it = _modules.begin(); it != _modules.end() && ret == UpdateStatus::Continue; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(auto it = _modules.begin(); it != _modules.end() && ret == UpdateStatus::Continue; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(getTimer().getDeltaTime());

	for(auto it = _modules.begin(); it != _modules.end() && ret == UpdateStatus::Continue; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(auto it = _modules.rbegin(); it != _modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();
	return ret;
}
