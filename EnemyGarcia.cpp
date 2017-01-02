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

bool EnemyGarcia::Init() {
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

//----------------------- position and speed ---------------------------

	return true;
}
