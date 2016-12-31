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
		//UpdateCurrentAnimation(being_knocked);
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
		if (upd_logic == true)
		{
			if (blocking_animation_remaining_cycles >= 0)
				blocking_animation_remaining_cycles -= 1;
			
			if (grounded == false)	
			{
				jump_remaining_cycles -= 1;
				if (jump_remaining_cycles == 3)
				{
					App->audio->PlayFx(fx_landing_jump);
					UpdateCurrentAnimation(jump);	
					blocking_animation_remaining_cycles = 2;
				}
				grounded = (jump_remaining_cycles <= 0) ? true : false;
				
			}
		}
		
		if (AllowAnimationInterruption())
		{
			move_speed = iPoint(0, 0);
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

			if (grounded == false)
			{
				// air move! 
				if (upd_logic)
				{
					position.x += move_speed.x;
					position.y -= 1;
				}
				if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
				{
					UpdateCurrentAnimation(jump_attack);
				}
			}
			else
			{
				if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
				{
					grounded = false;
					ground_y = position.y;

					jump_remaining_cycles = jump_duration;
					blocking_animation_remaining_cycles = 5;
					
					UpdateCurrentAnimation(jump);
					App->audio->PlayFx(fx_jump);
				}
				else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
				{
					if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) // back attack
					{
						blocking_animation_remaining_cycles = attacks_duration;
						UpdateCurrentAnimation(attack_back);
					}
					else  //combo attacks 
					{
						blocking_animation_remaining_cycles = attacks_duration;
						UpdateCurrentAnimation(attack3);
					}
					App->audio->PlayFx(fx_attack_miss);
				}
				else
				{
					if (upd_logic)
					{
						if (move_speed.IsZero())
						{
							UpdateCurrentAnimation(idle);
						}
						else
						{
							position += move_speed;
							UpdateCurrentAnimation(walk);
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


	//animation durations
	attacks_duration = (int)json_object_dotget_number(root_object, "player.duration.attacks");
	jump_duration = (int)json_object_dotget_number(root_object, "player.duration.jump");


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

	//jump_attack animation
	jump_attack.speed = (float)json_object_dotget_number(root_object, "player.jump_attack.speed");
	j_array = json_object_dotget_array(root_object, "player.jump_attack.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		jump_attack.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
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

	//attack3 animation
	attack3.speed = (float)json_object_dotget_number(root_object, "player.attack3.speed");
	j_array = json_object_dotget_array(root_object, "player.attack3.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		attack3.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);
	
	position = iPoint(50, 200);
	
	// fx sound load
	if (json_object_dothas_value_of_type(root_object, "player.fx.life_up", JSONString))
		fx_extra_life = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.life_up"));
	if (json_object_dothas_value_of_type(root_object, "player.fx.attack_miss", JSONString))
		fx_attack_miss = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.attack_miss"));
	if (json_object_dothas_value_of_type(root_object, "player.fx.jump", JSONString))
		fx_jump = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.jump"));
	if (json_object_dothas_value_of_type(root_object, "player.fx.jump_land", JSONString))
		fx_landing_jump= App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.jump_land"));

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