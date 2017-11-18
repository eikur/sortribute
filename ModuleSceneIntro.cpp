#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleUI.h"
#include "EntityManager.h"

#include "ConfigurationLoader.h"
#include "Timer.h"

#include "ModuleSceneIntro.h"

ModuleSceneIntro::ModuleSceneIntro(bool active) : Module(active)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Init()
{
	return true; 
}
// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading intro screen");
	if (LoadConfigFromFile() == false)
	{
		LOG("Intro: failed to initialise");
		return false;
	}

	App->renderer->camera.x = App->renderer->camera.y = 0;
	if (App->manager->IsEnabled())
		App->manager->Disable();
	if (App->ui->IsEnabled())
		App->ui->Disable();
	App->audio->PlayMusic(music_path.c_str());
	elapsed_msec = 0;
	return true;
}

// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading intro scene");

	App->textures->Unload(background);
	
	return true;
}

// Update: draw background
UpdateStatus ModuleSceneIntro::Update()
{
	bool gamepad_attached = App->input->UsingGamepad();
	App->renderer->Blit(background, 0, 0, 0, false);

	elapsed_msec += App->timer->DeltaTime();

	if (elapsed_msec <= blink_msg_msec)
	{
		if (gamepad_attached == true)
			App->fonts->Print(102, 150, ModuleFonts::Fonts::hud_small, "PRESS 1P START");
		else
			App->fonts->Print(110, 150, ModuleFonts::Fonts::hud_small, "PRESS ENTER");
	}
	else if (elapsed_msec > 2 * blink_msg_msec)
		elapsed_msec = 0;

	if ( App->fade->isFading() == false && 
		(	(gamepad_attached == true && App->input->GetGamepadButton(GamepadButton::START) == KEY_DOWN) ||
			(gamepad_attached == false && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN )
		))
	{
		App->fade->FadeToBlack((Module*)App->scene3, this);
		App->audio->PlayFx(fx_start);
	}

	return UpdateStatus::Continue;
}

bool ModuleSceneIntro::LoadConfigFromFile()
{
	JSON_Object *json_intro = App->config->GetJSONObject("intro");
	if (json_intro == nullptr) { return false;  }

	background = App->textures->Load(App->config->GetStringFromJSONObject(json_intro, "graphics_file"));
	if (background == nullptr) { return false; }

	music_path = App->config->GetStringFromJSONObject(json_intro, "music_file");
	if (music_path == "") { return false; }

	fx_start = App->audio->LoadFx(App->config->GetStringFromJSONObject(json_intro, "fx_start"));
	if (fx_start == -1) { return false; }

	blink_msg_msec = App->config->GetIntFromJSONObject(json_intro, "blink_msg_msec");
	if (blink_msg_msec == 0) { return false; }

	return true;
}