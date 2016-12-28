#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "Animation.h"
#include "ModuleScene3.h"

ModuleScene3::ModuleScene3(bool active) : Module(active)
{
}

ModuleScene3::~ModuleScene3()
{}


bool ModuleScene3::Init()
{
	LOG("Scene3: Init from config file\n");

	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Scene3: Unable to load config from file\n");
		return false;
	}

	return true;
}

bool ModuleScene3::Start()
{
	LOG("Scene3: Starting MoonBeach\n");
	App->audio->PlayMusic(music_path.c_str(), 1.0F);
	return true;
}



update_status ModuleScene3::Update()
{
	//Draw everything
	App->renderer->Blit(graphics, back_x, back_y, NULL);
	return UPDATE_CONTINUE;
}

bool ModuleScene3::CleanUp()
{
	LOG("Scene3: Unloading Moon Beach scene\n");
	App->textures->Unload(graphics);

	return true;
}

bool ModuleScene3::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;

	if (json_object_dothas_value_of_type(json_object(root_value), "scene3.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(json_object(root_value), "scene3.graphics_file"));
	
	if (json_object_dothas_value_of_type(json_object(root_value), "scene3.music_file",JSONString))
		music_path = json_object_dotget_string(json_object(root_value), "scene3.music_file");

	back_x = (int)json_object_dotget_number(json_object(root_value), "scene3.x");
	back_y = (int)json_object_dotget_number(json_object(root_value), "scene3.y");

	json_value_free(root_value);

	if (graphics == nullptr || music_path == "")
	{
		if (graphics != nullptr)
			App->textures->Unload(graphics);
		return false;
	}
	
	return true;
}