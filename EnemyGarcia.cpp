#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "EntityManager.h"

#include "EnemyGarcia.h"

EnemyGarcia::EnemyGarcia(EntityManager* parent): Entity(Entity::Types::npc_garcia), parent(parent)
{ }

EnemyGarcia::~EnemyGarcia(){}

bool EnemyGarcia::Init() 
{
	if (LoadFromConfigFile(CONFIG_FILE) == false)
	{
		LOG("Error loading player config from file");
		return false;
	}
	UpdateCurrentAnimation(&idle);
	return true;
}

bool EnemyGarcia::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	if (upd_logic)
	{
		if (parent->player->position.x >= position.x)
			facing_right = true;
		else
			facing_right = false;
	}
	UpdateCurrentAnimation(&idle);
	
	if (facing_right)
	{
		App->renderer->Blit(graphics, position.x + sprite_offset.x, position.y + sprite_offset.y, &(current_animation->GetCurrentFrame()), 1.0F, false);
	}
	else
	{
		App->renderer->Blit(graphics, position.x + sprite_offset_flip.x, position.y + sprite_offset_flip.y, &(current_animation->GetCurrentFrame()), 1.0F, true);
	}


	return UPDATE_CONTINUE;
}

void EnemyGarcia::Die() {
	// TODO: Implement death
}

bool EnemyGarcia::LoadFromConfigFile(const char* file_path) 
{
	JSON_Value *root_value;
	JSON_Object *root_object;
	JSON_Array *j_array;
	JSON_Array *j_array_inner;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	if (json_object_dothas_value_of_type(root_object, "garcia.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(root_object, "garcia.graphics_file"));
	if (graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

//----------------------- ---------------------------
//----------------------- sprites ---------------------------
	j_array = json_object_dotget_array(root_object, "garcia.sprite_offset");
	sprite_offset.x = (int)json_array_get_number(j_array, 0);
	sprite_offset.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "garcia.sprite_offset_flip");
	sprite_offset_flip.x = (int)json_array_get_number(j_array, 0);
	sprite_offset_flip.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	//idle animation
	idle.speed = (float)json_object_dotget_number(root_object, "garcia.idle.speed");
	j_array = json_object_dotget_array(root_object, "garcia.idle.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		idle.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

//--- free json 
	json_value_free(root_value);

	return true;
}
