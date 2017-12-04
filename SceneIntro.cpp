#include "Globals.h"
#include "Application.h"
#include "TextureHelper.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleUI.h"
#include "EntityManager.h"

#include "ConfigurationLoader.h"
#include "Timer.h"

#include "SceneIntro.h"

SceneIntro::SceneIntro(SceneManager& sceneManager) : Scene(sceneManager)
{}

SceneIntro::~SceneIntro()
{}

bool SceneIntro::Init()
{
	LOG("Loading intro screen");
	if (LoadConfigFromFile() == false)
	{
		LOG("Intro: failed to initialise");
		return false;
	}

	return true; 
}
// Load assets
bool SceneIntro::Start()
{
	App->getRenderer().camera.x = App->getRenderer().camera.y = 0;
	if (App->getEntityManager().IsEnabled())
		App->getEntityManager().Disable();
	if (App->getUI().IsEnabled())
		App->getUI().Disable();
	App->getAudio().PlayMusic(music_path.c_str());
	elapsed_msec = 0;
	return true;
}

// UnLoad assets
bool SceneIntro::CleanUp()
{
	LOG("Unloading intro scene");

	return true;
}

// Update: draw background
UpdateStatus SceneIntro::Update(float)
{
	bool gamepad_attached = App->getInput().UsingGamepad();
	App->getRenderer().Blit(background, 0, 0, 0, false);

	elapsed_msec += App->getTimer().getDeltaTime();

	if (elapsed_msec <= blink_msg_msec)
	{
		if (gamepad_attached == true)
			App->getFonts().Print(102, 150, ModuleFonts::FontType::HudSmall, "PRESS 1P START");
		else
			App->getFonts().Print(110, 150, ModuleFonts::FontType::HudSmall, "PRESS ENTER");
	}
	else if (elapsed_msec > 2 * blink_msg_msec)
	{
		elapsed_msec = 0;
	}
	
	if ((gamepad_attached == true && App->getInput().GetGamepadButton(GamepadButton::START) == KEY_DOWN) ||
		(gamepad_attached == false && App->getInput().GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN))
	{
		getManager().SwapScene(SceneManager::SceneId::Stage3);
		App->getAudio().PlayFx(fx_start);
	}

	return UpdateStatus::Continue;
}

bool SceneIntro::LoadConfigFromFile()
{
	JSON_Object *json_intro = App->getConfig().GetJSONObject("intro");
	if (json_intro == nullptr) { return false;  }

	background = App->getTextures().Load(App->getConfig().GetStringFromJSONObject(json_intro, "graphics_file"));
	if (background == nullptr) { return false; }

	music_path = App->getConfig().GetStringFromJSONObject(json_intro, "music_file");
	if (music_path == "") { return false; }

	fx_start = App->getAudio().LoadFx(App->getConfig().GetStringFromJSONObject(json_intro, "fx_start"));
	if (fx_start == -1) { return false; }

	blink_msg_msec = App->getConfig().GetIntFromJSONObject(json_intro, "blink_msg_msec");
	if (blink_msg_msec == 0) { return false; }

	return true;
}