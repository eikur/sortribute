#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

#include "Player.h"


Player::Player() : Entity(Types::player) {}
Player::~Player(){}

bool Player::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		position.y -= speed.y;
		
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			position.y += speed.y;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		position.x -= speed.x;
		facing_right = false;
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			position.x += speed.x;
			facing_right = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE)
	{
		if (current_animation != &idle)
			idle.Reset();
		current_animation = &idle;
	}
	else
	{
		if (current_animation != &walk)
			walk.Reset();
		current_animation = &walk;
	}
	
	//draw after computing the logic
	App->renderer->Blit(graphics, position.x + sprite_offset.x, position.y + sprite_offset.y, &(current_animation->GetCurrentFrame()), 1.0F, !facing_right);
	return true;
}

bool Player::LoadFromConfigFile(const char* file_path)
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

	if (json_object_dothas_value_of_type(root_object, "player.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(root_object, "player.graphics_file"));
	if (graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

	j_array = json_object_dotget_array(root_object, "player.speed");
	speed.x = (int)json_array_get_number(j_array, 0);
	speed.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	health = (int)json_object_dotget_number(root_object, "player.health");
	
	j_array = json_object_dotget_array(root_object, "player.sprite_offset");
	sprite_offset.x = (int)json_array_get_number(j_array, 0);
	sprite_offset.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	idle.speed = (float)json_object_dotget_number(root_object, "player.idle.speed");
	j_array = json_object_dotget_array(root_object, "player.idle.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		idle.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);
	
	walk.speed = (float)json_object_dotget_number(root_object, "player.walk.speed");
	j_array = json_object_dotget_array(root_object, "player.walk.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		walk.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	position = iPoint(50, 100);
	json_value_free(root_value);
	
	return true;

}
