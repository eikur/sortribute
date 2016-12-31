#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "Player.h"


Player::Player() : Entity(Types::player) {}
Player::~Player(){}

bool Player::Update(unsigned int msec_elapsed, const bool upd_logic)
{
if (IsAlive() == false)
	{
		/*
		if (current_animation!= &being_knocked)
		{
			current_animation = &being_knocked;
			current_animation->Reset();
		}
		*/
		ModifyLives(-1);
		if (lives > 0)
		{
			LOG("Reraise!\n");
		}//ReRaise();}
		else
		{
			LOG("Die...\n");
		} 		//	Die();
	}
	else
	{
		if (AllowAnimationInterruption() == false)
		{
			//update remaining time
			blocking_animation_remaining_cycles -= 1;
			if (blocking_animation_remaining_cycles <= 0)
				blocking_animation_remaining_cycles = 0;
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
			{
				if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) // back attack
				{
					blocking_animation_remaining_cycles = attacks_duration;
					if (current_animation != &attack_back)
					{
						current_animation = &attack_back;
						current_animation->Reset();
					}
				}
				else
				{
					blocking_animation_remaining_cycles = attacks_duration;
					if (current_animation != &attack1)
					{
						current_animation = &attack1;
						current_animation->Reset();
					}
				}
			}
			else
			{
				if (upd_logic)
				{
					iPoint move_speed = iPoint(0, 0);
					if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
						move_speed.y -= speed.y;
					else
						if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
							move_speed.y += speed.y;

					if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
					{
						move_speed.x -= speed.x;
						facing_right = false;
					}
					else
					{
						if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
						{
							move_speed.x += speed.x;
							facing_right = true;
						}
					}

					if (move_speed.IsZero())
					{
						if (current_animation != &idle)
						{
							current_animation = &idle;
							current_animation->Reset();
						}
					}
					else
					{
						position += move_speed;

						if (current_animation != &walk)
						{
							current_animation = &walk;
							current_animation->Reset();
						}
					}
				}
			}
		}
	}

	//draw the sprites after the logic calculations
	if (facing_right) 
		App->renderer->Blit(graphics, position.x + sprite_offset.x, position.y + sprite_offset.y, &(current_animation->GetCurrentFrame()), 1.0F, false);
	else
		App->renderer->Blit(graphics, position.x + sprite_offset_flip.x, position.y + sprite_offset_flip.y, &(current_animation->GetCurrentFrame()), 1.0F, true);

	// miscelaneous
	CheatCodes();
	
	return true;
}



void Player::AddScore(int addition)
{
	if ((score + addition) >= 999999) {
		score = 999999;
		return;
	}
	if (score < 50000 && (score+addition)>=50000)
	{ 
		ModifyLives(+1);
	}
	score += addition;
}

void Player::ModifyLives(int mod_to_add)
{
	lives += mod_to_add;
	if (lives> 9)
	{
		lives = 9;
		return;
	}
	if (lives<= 0)
	{
		lives = 0;
		return;		//Game over
	}
	if (mod_to_add > 0)
		App->audio->PlayFx(fx_extra_life);
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

	//move speed
	j_array = json_object_dotget_array(root_object, "player.speed");
	speed.x = (int)json_array_get_number(j_array, 0);
	speed.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);
	
	//health
	health = (int)json_object_dotget_number(root_object, "player.health");
	//load lives and such

	//sprite_offsets
	j_array = json_object_dotget_array(root_object, "player.sprite_offset");
	sprite_offset.x = (int)json_array_get_number(j_array, 0);
	sprite_offset.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "player.sprite_offset_flip");
	sprite_offset_flip.x = (int)json_array_get_number(j_array, 0);
	sprite_offset_flip.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	//idle animation
	idle.speed = (float)json_object_dotget_number(root_object, "player.idle.speed");
	j_array = json_object_dotget_array(root_object, "player.idle.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		idle.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);
	
	//walk animation
	walk.speed = (float)json_object_dotget_number(root_object, "player.walk.speed");
	j_array = json_object_dotget_array(root_object, "player.walk.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		walk.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	//jump animation
	jump.speed = (float)json_object_dotget_number(root_object, "player.jump.speed");
	j_array = json_object_dotget_array(root_object, "player.jump.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		jump.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	//attack1 animation
	attack1.speed = (float)json_object_dotget_number(root_object, "player.attack1.speed");
	j_array = json_object_dotget_array(root_object, "player.attack1.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		attack1.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);
	
	//attack2 animation
	attack2.speed = (float)json_object_dotget_number(root_object, "player.attack2.speed");
	j_array = json_object_dotget_array(root_object, "player.attack2.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		attack2.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	
	attacks_duration = (int)json_object_dotget_number(root_object, "player.attacks_duration");
	
	position = iPoint(50, 200);
	
	if (json_object_dothas_value_of_type(root_object, "player.fx.life_up", JSONString))
		fx_extra_life = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.life_up"));

	json_value_free(root_value);
	
	return true;

}

void Player::CheatCodes() {
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
		AddScore(1000);
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		ModifyLives(+1);
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && help < 9)
		help += 1;
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		health = 0;
}