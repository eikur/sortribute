#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
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
	return true;
}



update_status ModuleScene3::Update()
{
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

	graphics = App->textures->Load(json_object_dotget_string(json_object(root_value), "scene3.graphics_file"));
	
	if (json_object_dothas_value_of_type(json_object(root_value), "scene3.music_file",JSONString))
		music_path = json_object_dotget_string(json_object(root_value), "scene3.music_file");

	json_value_free(root_value);

	if (graphics == nullptr || music_path == "")
	{
		graphics = nullptr;	
		return false;
	}
	
	return true;
}