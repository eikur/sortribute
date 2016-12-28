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
	App->renderer->Blit(graphics, foreground_pos.x, foreground_pos.y, &foreground_section);
	App->renderer->Blit(graphics, background_pos.x, background_pos.y, &background_section);
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
	JSON_Array *j_array_pos; 
	JSON_Array *j_array_section;

	if (root_value == nullptr)
		return false;

	if (json_object_dothas_value_of_type(json_object(root_value), "scene3.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(json_object(root_value), "scene3.graphics_file"));
	
	if (json_object_dothas_value_of_type(json_object(root_value), "scene3.music_file",JSONString))
		music_path = json_object_dotget_string(json_object(root_value), "scene3.music_file");

	//background load
	j_array_pos = json_object_dotget_array(json_object(root_value), "scene3.background.position");
	j_array_section = json_object_dotget_array(json_object(root_value), "scene3.background.section");

	background_pos.x = (int)json_array_get_number(j_array_pos, 0);
	background_pos.y = (int)json_array_get_number(j_array_pos, 1);
	background_section.x = (int)json_array_get_number(j_array_section, 0);
	background_section.y = (int)json_array_get_number(j_array_section, 1);
	background_section.w = (int)json_array_get_number(j_array_section, 2);
	background_section.h= (int)json_array_get_number(j_array_section, 3);

	json_array_clear(j_array_pos);
	json_array_clear(j_array_section);

	//foreground load
	j_array_pos = json_object_dotget_array(json_object(root_value), "scene3.foreground.position");
	j_array_section = json_object_dotget_array(json_object(root_value), "scene3.foreground.section");

	foreground_pos.x = (int)json_array_get_number(j_array_pos, 0);
	foreground_pos.y = (int)json_array_get_number(j_array_pos, 1);
	foreground_section.x = (int)json_array_get_number(j_array_section, 0);
	foreground_section.y = (int)json_array_get_number(j_array_section, 1);
	foreground_section.w = (int)json_array_get_number(j_array_section, 2);
	foreground_section.h = (int)json_array_get_number(j_array_section, 3);

	json_array_clear(j_array_pos);
	json_array_clear(j_array_section);

	//wave_sand load



	json_value_free(root_value);

	if (graphics == nullptr || music_path == "")
	{
		if (graphics != nullptr)
			App->textures->Unload(graphics);
		return false;
	}
	
	return true;
}