#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "EntityManager.h"

#include "ModuleSceneIntro.h"

ModuleSceneIntro::ModuleSceneIntro(bool active) : Module(active)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading intro screen");
	App->renderer->camera.x = App->renderer->camera.y = 0;
	if (App->manager->IsEnabled())
		App->manager->Disable();
	return LoadConfigFromFile(CONFIG_FILE);
}

// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading space scene");

	App->textures->Unload(background);
	
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0,0,false);
	App->fonts->Print(110, 150, ModuleFonts::Fonts::hud_small, "PRESS ENTER");
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && App->fade->isFading() == false)
	{
 		App->fade->FadeToBlack((Module*)App->scene3, this);
	}
	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	if (json_object_dothas_value_of_type(root_object, "intro.graphics_file", JSONString))
		background = App->textures->Load(json_object_dotget_string(root_object, "intro.graphics_file"));
	json_value_free(root_value);

	if (background == nullptr)
		return false;
	else 
		return true;
}