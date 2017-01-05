#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

#include "Player.h"



Player::Player() : Entity(Types::player) {}
Player::~Player(){}

bool Player::Init()
{
	if (LoadFromConfigFile(CONFIG_FILE) == false)
	{
		LOG("Error loading player config from file");
		return false;
	}
	ReRaise();
	return true;
}

bool Player::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	if (IsAlive() == false)
	{
		if (blocking_animation_remaining_msec > 0) {
			blocking_animation_remaining_msec -= msec_elapsed;
			if (upd_logic && current_animation == &being_knocked)
				UpdatePosition({facing_right? -2 : 2,0});
		}

		if (blocking_animation_remaining_msec <= 0 && current_animation != &dying)
			UpdateCurrentAnimation(&dying, dying_duration);
		if (blocking_animation_remaining_msec <= 0 && current_animation == &dying)
		{
			ModifyLives(-1);
			if (lives > 0)
				ReRaise();
			else
			{
				RemoveColliders();
				return false;
			}
		}
	}
	else
	{
		GetInput(upd_logic);
		move_speed = { 0,0 };

		if (upd_logic == true)
		{
			if (blocking_animation_remaining_msec > 0)
				blocking_animation_remaining_msec -= msec_elapsed;

			// animation transitions
			if (blocking_animation_remaining_msec <= 0)
			{
				if (current_animation == &jump_prep)
				{
					UpdateCurrentAnimation(&jump);
					air_remaining_msec = jump_duration;
				}
				else if (current_animation == &jump_land || current_animation == &being_hit || current_animation == &take_item)
					UpdateCurrentAnimation(&idle);
				else if (current_animation == &being_knocked || current_animation == &being_thrown_front)
					UpdateCurrentAnimation(&standing_up, standing_up_duration);
				else if (current_animation == &holding_front_attack)
					UpdateCurrentAnimation(&holding_front);
				else if (current_animation == &holding_swap)
				{
					is_holding_back = !is_holding_back;	is_holding_front = !is_holding_front; 	facing_right = !facing_right;
					position.y = ground_y;
					current_hold_swaps += 1;
					if (current_hold_swaps < max_hold_swaps)
					{
						if (is_holding_front)
						{
							held_entity->SetBeingHoldFront();	UpdateCurrentAnimation(&holding_front, 0, fx_landing_jump);
						}
						else if (is_holding_back)
						{
							held_entity->SetBeingHoldBack(); UpdateCurrentAnimation(&holding_back, 0, fx_landing_jump);
						}
					}
					else
					{
						current_hold_swaps = 0;
						held_entity->SetIdle();
						held_entity = nullptr;
						UpdateCurrentAnimation(&idle);
					}
				}
			}

			if( combo_remaining_msec > 0)
				combo_remaining_msec -= msec_elapsed;
			if (combo_remaining_msec <= 0)
			{
				combo_remaining_msec = 0;
				current_combo_hits = 0;
			}

			if (grounded == false)
			{
				if (air_remaining_msec > 0)
				{
					
					if (respawn_fall == true)	
					{
						air_remaining_msec -= msec_elapsed;
						move_speed.y += 9;
					}
					else if (jumping)
					{
						air_remaining_msec -= msec_elapsed;
						int divisor =  jump_duration / 16;
						int frames_left = air_remaining_msec / divisor;

						switch (frames_left)
						{
						case 0:	 move_speed.y += 7; break;
						case 1:	 move_speed.y += 6; break;
						case 2:	 move_speed.y += 5; break;
						case 3:	 move_speed.y += 5; break;
						case 4:	 move_speed.y += 4; break;
						case 5:	 move_speed.y += 3; break;
						case 6:	 move_speed.y += 2; break;
						case 7:  move_speed.y += 1; break;
						case 8:  move_speed.y -= 1; break;
						case 9:  move_speed.y -= 2; break;
						case 10: move_speed.y -= 3; break;
						case 11: move_speed.y -= 4; break;
						case 12: move_speed.y -= 5; break;
						case 13: move_speed.y -= 5; break;
						case 14: move_speed.y -= 6; break;
						case 15: move_speed.y -= 7; break;
						}
					}
					else if (is_holding_back || is_holding_front)
					{
						air_remaining_msec -= msec_elapsed;
						int divisor = holding_swap_duration / 5;
						int frames_left = air_remaining_msec / divisor;
						int mod = facing_right ? 1 : -1;

						holding_swap.SetCurrentFrame(4 - frames_left);
						switch (frames_left)
						{
						case 4: SetPosition({ held_entity->position.x - mod*24, held_entity->position.y - 6 }); break;
						case 3: SetPosition({ held_entity->position.x, held_entity->position.y - 50 }); break;
						case 2: SetPosition({ held_entity->position.x, held_entity->position.y - 50}); break;
						case 1: SetPosition({ held_entity->position.x, held_entity->position.y - 50 }); break;
						case 0: SetPosition({ held_entity->position.x + mod*24 , held_entity->position.y - 6 }); break;
						}
					}
					else
					{
						// falling after knockout or throw
					}

				}
				if (air_remaining_msec <= 0)
				{
					respawn_fall = false;
					move_speed.y = 0;
					position.y = ground_y;
					UpdateCurrentAnimation(&jump_land, jump_prep_duration, fx_landing_jump);
				}
			}
		}

		if (AllowAnimationInterruption())
		{
			if (is_holding_front)
			{
				if (held_entity->health <= 0)
				{
					held_entity = nullptr;
					UpdateCurrentAnimation(&idle);
				}
				else if (input_hold_front_throw)
				{
					facing_right = !facing_right;
					UpdateCurrentAnimation(&throwing_front, throwing_duration);
					held_entity->SetBeingThrownFront();
					held_entity = nullptr;
				}
				else if (input_attack )
				{
					current_combo_hits += 1;
					combo_remaining_msec = combo_window_msec;
					if (current_combo_hits <= 2)
					{
						held_entity->SetBeingHoldFrontHit(attack1_dmg);
						UpdateCurrentAnimation(&holding_front_attack, attacks_duration, fx_attack_hit);
					}
					else
					{
						held_entity->SetBeingHoldFrontHit(attack2_dmg);									 // tmp
						UpdateCurrentAnimation(&holding_front_attack2, attacks_duration, fx_attack_hit); // tmp
					}
						
				}
				else if (input_jump)
				{
					UpdateCurrentAnimation(&holding_swap, holding_swap_duration, fx_jump);
					air_remaining_msec = holding_swap_duration;
				}
				else if ((facing_right == true && input_horizontal < 0 )|| (facing_right == false && input_horizontal > 0))
				{
					held_entity->SetIdle();
					held_entity = nullptr;
					UpdateCurrentAnimation(&idle);
					facing_right = !facing_right;
				}
			}
			else if (is_holding_back)
			{
				if (input_jump)
				{
					UpdateCurrentAnimation(&holding_swap, holding_swap_duration, fx_jump);
					air_remaining_msec = holding_swap_duration;
				}
				else if (input_attack)
				{
					UpdateCurrentAnimation(&throwing_back, throwing_duration);
				}
				else if ((facing_right == true && input_horizontal < 0) || (facing_right == false && input_horizontal > 0))
				{
					held_entity->SetIdle();
					held_entity = nullptr;
					UpdateCurrentAnimation(&idle);
					facing_right = !facing_right;
				}
			}
			else if (is_being_hold_back)
			{

			}
			else
			{
				move_speed.y += grounded? speed.y*input_vertical : 0;
				move_speed.x += speed.x*input_horizontal;
				facing_right = input_horizontal == 0 ? facing_right : (input_horizontal > 0 ? true : false);

				if (grounded == false)	
				{
					if (input_attack)
						UpdateCurrentAnimation(&jump_attack, 0, fx_voice);
				}
				else 		
				{
					if (input_attack_back)
						UpdateCurrentAnimation(&attack_back, attacks_duration, fx_attack_miss);
					else if (input_jump)
						UpdateCurrentAnimation(&jump_prep, jump_prep_duration, fx_jump);
					else if (input_attack)
					{
						if (current_combo_hits <= 2)
							UpdateCurrentAnimation(&attack1, attacks_duration, fx_attack_miss);
						else  if (current_combo_hits == 3)
							UpdateCurrentAnimation(&attack2, attacks_duration, fx_attack_miss);
						else if (current_combo_hits == 4)
						{
							UpdateCurrentAnimation(&attack3, attacks_duration, fx_attack_miss);
							current_combo_hits = 0;
						}
					}
					else if (move_speed.IsZero())
						UpdateCurrentAnimation(&idle);
					else
						UpdateCurrentAnimation(&walk);
				}
			}
		}
		if (upd_logic)
		{
			UpdatePosition(move_speed);
			App->renderer->MoveCamera(position.x, speed.x);
		}

		// miscelaneous
		CheatCodes();
	}



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

void Player::ReRaise()
{
	position = { 40, 32};
	ground_y = 174;
	UpdateCurrentAnimation(&jump, jump_duration);
	facing_right = true;
	respawn_fall = true;
	air_remaining_msec = jump_duration;

	health = max_health;
}

void Player::UpdatePosition(const iPoint new_speed) {

	if (grounded)
		ground_y = position.y;

	position += new_speed;

	App->renderer->GetPlayerPositionLimits(position_limits);

	int up = position_limits.y;
	int down = up + position_limits.h;
	int left = position_limits.x;
	int right = left + position_limits.w;

	if (position.x > right)
		position.x = right;
	else
		if (position.x < left)
			position.x = left;
	if (grounded)
	{
		if (position.y < up)
			position.y = up;
		else
			if (position.y > down)
				position.y = down;
		ground_y = position.y;
	}

	//apply offset to colliders
	if (facing_right)
	{
		attack_collider->rect.x = position.x + attack_collider_offset.x;
		hit_collider->rect.x = position.x + hit_collider_offset.x;
	}
	else
	{
		attack_collider->rect.x = position.x + -(attack_collider_offset.x + attack_collider->rect.w);
		hit_collider->rect.x = position.x - (hit_collider_offset.x + hit_collider->rect.w);
	}
	attack_collider->rect.y = position.y + attack_collider_offset.y;	
	hit_collider->rect.y = position.y + hit_collider_offset.y;
}

void Player::GetInput( bool upd_logic )
{
	if (upd_logic)
		ResetInput();
	
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		input_queue.append("u");
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		input_queue.append("d");
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		input_queue.append("l");
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		input_queue.append("r");
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		input_queue.append("A");
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT)
		input_queue.append("a");
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		input_queue.append("B");
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT)
		input_queue.append("b");
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		input_queue.append("C");
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
		input_queue.append("c");
	
	// find in the string
	if (input_queue.find("u") != std::string::npos)
		input_vertical = -1;
	else
		if (input_queue.find("d") != std::string::npos)
			input_vertical = 1;
	
	if (input_queue.find("l") != std::string::npos)
		input_horizontal = -1;
	else
		if (input_queue.find("r") != std::string::npos)
			input_horizontal = 1;

	if ( (( input_queue.find("lB") != std::string::npos || input_queue.find("lb") != std::string::npos) && facing_right) ||
			((input_queue.find("rB") != std::string::npos || input_queue.find("rb") != std::string::npos) && !facing_right) )
		input_hold_front_throw = true;
	else if (input_queue.find("BC") != std::string::npos || input_queue.find("bC") != std::string::npos || input_queue.find("Bc") != std::string::npos)
		input_attack_back = true;
	else
	{
		if (input_queue.find("B") != std::string::npos)
			input_attack = true;
		if (input_queue.find("C") != std::string::npos)
			input_jump = true;
	}

	if (input_queue.find("A") != std::string::npos)
		input_help = true;
}

void Player::ResetInput()
{
	input_queue = "";
	input_horizontal = input_vertical = 0;
	input_help = input_attack = input_jump = input_attack_back = input_hold_front_throw = false;
}
//------------------------------------------------------------------------
bool Player::LoadFromConfigFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

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

//----------------------- game variables ---------------------------
	LoadiPointFromJSONObject(root_object, "player.speed", &speed);
	max_health = (int)json_object_dotget_number(root_object, "player.max_health");
	health = max_health;
	lives = (int)json_object_dotget_number(root_object, "player.lives");
	score = (int)json_object_dotget_number(root_object, "player.score");
	help = (int)json_object_dotget_number(root_object, "player.help");

	//----------------------- damages ---------------------------

	attack1_dmg = (int)json_object_dotget_number(root_object, "player.damage.attack1");
	attack2_dmg = (int)json_object_dotget_number(root_object, "player.damage.attack2");
	attack3_dmg = (int)json_object_dotget_number(root_object, "player.damage.attack3");
	attack_back_dmg = (int)json_object_dotget_number(root_object, "player.damage.attack_back");
	throw_dmg = (int)json_object_dotget_number(root_object, "player.damage.throw");


//----------------------- animation durations---------------------------
	attacks_duration = (int)json_object_dotget_number(root_object, "player.duration.attacks");
	jump_prep_duration = (int)json_object_dotget_number(root_object, "player.duration.jump_prep");
	jump_duration = (int)json_object_dotget_number(root_object, "player.duration.jump");
	throwing_duration = (int)json_object_dotget_number(root_object, "player.duration.throwing");
	being_hit_duration = (int)json_object_dotget_number(root_object, "player.duration.being_hit");
	being_thrown_duration = (int)json_object_dotget_number(root_object, "player.duration.being_thrown");
	being_knocked_duration= (int)json_object_dotget_number(root_object, "player.duration.being_knocked");
	standing_up_duration = (int)json_object_dotget_number(root_object, "player.duration.standing_up");
	holding_swap_duration = (int)json_object_dotget_number(root_object, "player.duration.holding_swap");
	combo_window_msec = (int)json_object_dotget_number(root_object, "player.duration.combo_window");
	dying_duration = (int)json_object_dotget_number(root_object, "player.duration.dying");

//----------------------- colliders ---------------------------
	hit_collider = LoadColliderFromJSONObject(root_object, "player.colliders.hit", colliderType::PLAYER, &hit_collider_offset);
	attack_collider = LoadColliderFromJSONObject(root_object, "player.colliders.attack", colliderType::PLAYER_ATTACK, &attack_collider_offset);

//----------------------- sprite offsets ---------------------------
	LoadiPointFromJSONObject(root_object, "player.sprite_offset", &sprite_offset);
	LoadiPointFromJSONObject(root_object, "player.sprite_offset_flip", &sprite_offset_flip);

// ------------------ animations -------------------------------------
	LoadAnimationFromJSONObject(root_object, "player.idle", &idle);
	LoadAnimationFromJSONObject(root_object, "player.walk", &walk);
	LoadAnimationFromJSONObject(root_object, "player.jump_prep", &jump_prep);
	LoadAnimationFromJSONObject(root_object, "player.jump", &jump);
	LoadAnimationFromJSONObject(root_object, "player.jump_attack", &jump_attack);
	jump_land = jump_prep;
	LoadAnimationFromJSONObject(root_object, "player.attack1", &attack1);
	LoadAnimationFromJSONObject(root_object, "player.attack2", &attack2);
	LoadAnimationFromJSONObject(root_object, "player.attack3", &attack3);
	LoadAnimationFromJSONObject(root_object, "player.attack_back", &attack_back);
	LoadAnimationFromJSONObject(root_object, "player.holding_front", &holding_front);
	LoadAnimationFromJSONObject(root_object, "player.holding_front_attack", &holding_front_attack);
	LoadAnimationFromJSONObject(root_object, "player.holding_front_attack2", &holding_front_attack2);
	LoadAnimationFromJSONObject(root_object, "player.holding_back", &holding_back);
	LoadAnimationFromJSONObject(root_object, "player.holding_swap", &holding_swap);
	LoadAnimationFromJSONObject(root_object, "player.throwing_front", &throwing_front);
	LoadAnimationFromJSONObject(root_object, "player.throwing_back", &throwing_back);
	LoadAnimationFromJSONObject(root_object, "player.take_item", &take_item);
	LoadAnimationFromJSONObject(root_object, "player.being_hit", &being_hit);
	LoadAnimationFromJSONObject(root_object, "player.being_knocked", &being_knocked);
	LoadAnimationFromJSONObject(root_object, "player.being_thrown_front", &being_thrown_front);
	LoadAnimationFromJSONObject(root_object, "player.standing_up", &standing_up);
	LoadAnimationFromJSONObject(root_object, "player.dying", &dying);

	LoadSDLRectFromJSONObject(root_object, "player.shadow", &shadow);

// ---------------------- sound effects ----------------------------
	LoadSoundFXFromJSONObject(root_object, "player.fx.voice", &fx_voice);
	LoadSoundFXFromJSONObject(root_object, "player.fx.life_up", &fx_extra_life);
	LoadSoundFXFromJSONObject(root_object, "player.fx.attack_miss", &fx_attack_miss);
	LoadSoundFXFromJSONObject(root_object, "player.fx.attack_hit", &fx_attack_hit);
	LoadSoundFXFromJSONObject(root_object, "player.fx.jump", &fx_jump);
	LoadSoundFXFromJSONObject(root_object, "player.fx.jump_land", &fx_landing_jump);
	LoadSoundFXFromJSONObject(root_object, "player.fx.death", &fx_death);

	json_value_free(root_value);

	return true;

}

void Player::CheatCodes() {
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		ModifyLives(+1);
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
		AddScore(1000);
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && help < 9)
		help += 1;
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		SetBeingHit(8);
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		UpdateCurrentAnimation(&throwing_back, throwing_duration);

}