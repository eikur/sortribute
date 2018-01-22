#include "Application.h"
#include "ConfigurationLoader.h"
#include "EntityManager.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "SceneManager.h"
#include "TextureFontsHelper.h"
#include "TextureHelper.h"
#include "Timer.h"

Application::Application()
{
	_config = std::make_unique<ConfigurationLoader>(CONFIG_FILE);
	_timer = std::make_unique<Timer>();
	_timer->TimerStart();
	_textures = std::make_unique<TextureHelper>();
	_fonts = std::make_unique<TextureFontsHelper>();

	// Base App modules
	_window = std::make_unique<ModuleWindow>();
	_renderer = std::make_unique<ModuleRender>();
	_input = std::make_unique<ModuleInput>();
	_audio = std::make_unique<ModuleAudio>();
	_ui = std::make_unique<ModuleUI>(false);
	//Specific game modules
	_entityManager = std::make_unique<EntityManager>(false);
	_sceneManager = std::make_unique<SceneManager>(getEntityManager());
	// Modules on top of game logic
	_collision = std::make_unique<ModuleCollision>(getEntityManager(), getSceneManager());
	_particles = std::make_unique<ModuleParticles>();

	// Order matters: they will init/start/pre/update/post in this order
	_modules = { _window.get(), _renderer.get(), _input.get(), _audio.get(), _ui.get(), _entityManager.get(), _collision.get(), _particles.get(), _sceneManager.get()};
}

Application::~Application()
{
	_sceneManager.reset();
	_particles.reset();
	_collision.reset();
	_entityManager.reset();
	_ui.reset();
	_fonts.reset();
	_audio.reset();
	_textures.reset();
	_input.reset();
	_renderer.reset();
	_window.reset();

	_fonts.reset();
	_timer.reset();
	_config.reset();
}

bool Application::Init()
{
	bool ret = true;

	for(auto it = _modules.begin(); it != _modules.end() && ret; ++it)
		ret = (*it)->Init();

	ret = _fonts->Init();

	for(auto it = _modules.begin(); it != _modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}
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
