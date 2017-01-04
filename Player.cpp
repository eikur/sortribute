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
		//UpdateCurrentAnimation(being_knocked);
		ModifyLives(-1);
		if (lives > 0)
			ReRaise();
		else
			return Die();
	}
	else
	{
		GetInput();
		move_speed = { 0,0 };

		if (upd_logic == true)
		{
			if (blocking_animation_remaining_msec > 0 )
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
				else if (current_animation == &being_knocked || current_animation == &being_thrown)
					UpdateCurrentAnimation(&standing_up, standing_up_duration);
				else if (current_animation == &holding_swap)
					UpdateCurrentAnimation(&holding_front);
			}


			// gravity calculations
			if (grounded == false)	
			{
				if (air_remaining_msec > 0)
				{
					if (respawn_fall == true)	// exception, just used once when respawning
					{
						
						move_speed.y += 9;
						air_remaining_msec -= msec_elapsed;
					}
					else if (jumping)
					{
						air_remaining_msec -= msec_elapsed;
						int divisor = jump_duration / 16;
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
						case 14: move_speed.y -= 7; break;
						case 15: move_speed.y -= 8; break;
						}
					}
					else {
						// falling after knockout or throw
					}
				}
				if (air_remaining_msec <= 0)
				{
					respawn_fall = false;
					if (position.y != ground_y)
					{
						move_speed.y = ground_y - position.y;
						UpdatePosition(move_speed);
					}
					UpdateCurrentAnimation(&jump_land, jump_prep_duration, fx_landing_jump);
				}
			}
		}
		
		if (AllowAnimationInterruption())
		{
			if(is_holding)
			{ 
				if (facing_right == true && input_horizontal < 0 || facing_right == false && input_horizontal > 0)	// letting go off the hold!
				{
					UpdateCurrentAnimation(&idle);
					facing_right = !facing_right;
				}
			}
			else if (is_being_hold)
			{

			}
			else
			{
				if (grounded)
					move_speed.y += speed.y*input_vertical;

				move_speed.x += speed.x*input_horizontal;
				facing_right = input_horizontal == 0 ? facing_right : (input_horizontal > 0 ? true : false);

				if (grounded == false)	// air 
				{
					if (input_attack)
						UpdateCurrentAnimation(&jump_attack, 0, fx_voice);
				}
				else 		// grounded
				{
					if (input_attack_back)
						UpdateCurrentAnimation(&attack_back, attacks_duration, fx_attack_miss);
					else if (input_jump)
						UpdateCurrentAnimation(&jump_prep, jump_prep_duration, fx_jump);
					else if (input_attack)
						UpdateCurrentAnimation(&attack1, attacks_duration, fx_attack_miss);
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

void Player::GetInput()
{
	ResetInput();
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		input_vertical = -1;
	else
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			input_vertical = 1;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		input_horizontal = -1;
	else
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			input_horizontal = 1;
		
	input_help = App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN;
	input_attack = App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN; 
	input_jump = App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN;
	input_attack_back = (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT) ||
		(App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN);

}

void Player::ResetInput()
{
	input_horizontal = input_vertical = 0;
	input_help = input_attack = input_jump = input_attack_back = false;
}
//------------------------------------------------------------------------
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

//----------------------- position and speed ---------------------------
	//move speed
	j_array = json_object_dotget_array(root_object, "player.speed");
	speed.x = (int)json_array_get_number(j_array, 0);
	speed.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);
	
//----------------------- game variables ---------------------------
	max_health = (int)json_object_dotget_number(root_object, "player.max_health");
	health = max_health;
	lives = (int)json_object_dotget_number(root_object, "player.lives");
	score = (int)json_object_dotget_number(root_object, "player.score");
	help = (int)json_object_dotget_number(root_object, "player.help");
//----------------------- animation durations---------------------------
	attacks_duration = (int)json_object_dotget_number(root_object, "player.duration.attacks");
	jump_prep_duration = (int)json_object_dotget_number(root_object, "player.duration.jump_prep");
	jump_duration = (int)json_object_dotget_number(root_object, "player.duration.jump");

//----------------------- colliders ---------------------------
	j_array = json_object_dotget_array(root_object, "player.colliders.hit");
	hit_collider_offset = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	hit_collider = App->collision->AddCollider( 
		{hit_collider_offset.x + position.x, hit_collider_offset.y +position.y, (int)json_array_get_number(j_array, 2) , (int)json_array_get_number(j_array, 3)}, 
		colliderType::PLAYER, *this);
	
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "player.colliders.attack");
	attack_collider_offset = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	attack_collider = App->collision->AddCollider(
		{attack_collider_offset.x + position.x, attack_collider_offset.y + position.y, (int)json_array_get_number(j_array, 2) , (int)json_array_get_number(j_array, 3) },
		colliderType::PLAYER_ATTACK, *this);
	json_array_clear(j_array);

//----------------------- sprites ---------------------------
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

	//jump preparation
	jump_prep.speed = (float)json_object_dotget_number(root_object, "player.jump_prep.speed");
	j_array = json_object_dotget_array(root_object, "player.jump_prep.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		jump_prep.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
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
	
	//jump landing animation
	jump_land = jump_prep;

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

	//attack back animation
	attack_back.speed = (float)json_object_dotget_number(root_object, "player.attack_back.speed");
	j_array = json_object_dotget_array(root_object, "player.attack_back.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		attack_back.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	//being_hit animation
	being_hit.speed = (float)json_object_dotget_number(root_object, "player.being_hit.speed");
	j_array = json_object_dotget_array(root_object, "player.being_hit.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		being_hit.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	//holding_front
	holding_front.speed = (float)json_object_dotget_number(root_object, "player.holding_front.speed");
	j_array = json_object_dotget_array(root_object, "player.holding_front.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		holding_front.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);
	
	//holding_front
	holding_back.speed = (float)json_object_dotget_number(root_object, "player.holding_back.speed");
	j_array = json_object_dotget_array(root_object, "player.holding_back.frames");
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		holding_back.frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	json_array_clear(j_array);

	//shadow
	j_array = json_object_dotget_array(root_object, "player.shadow");
	shadow = { (int)json_array_get_number(j_array,0), (int)json_array_get_number(j_array,1), (int)json_array_get_number(j_array,2), (int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

// ---------------------- sound effects ----------------------------
	if (json_object_dothas_value_of_type(root_object, "player.fx.voice", JSONString))
		fx_voice = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.voice"));
	if (json_object_dothas_value_of_type(root_object, "player.fx.life_up", JSONString))
		fx_extra_life = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.life_up"));
	if (json_object_dothas_value_of_type(root_object, "player.fx.attack_miss", JSONString))
		fx_attack_miss = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.attack_miss"));
	if (json_object_dothas_value_of_type(root_object, "player.fx.attack_hit", JSONString))
		fx_attack_hit = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.attack_hit"));
	if (json_object_dothas_value_of_type(root_object, "player.fx.jump", JSONString))
		fx_jump = App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.jump"));
	if (json_object_dothas_value_of_type(root_object, "player.fx.jump_land", JSONString))
		fx_landing_jump= App->audio->LoadFx(json_object_dotget_string(root_object, "player.fx.jump_land"));
	

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
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT)
		DecreaseHealth(4);
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		UpdateCurrentAnimation(&being_hit, being_hit_duration);
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		UpdateCurrentAnimation(&being_thrown, being_thrown_duration);
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		UpdateCurrentAnimation(&holding_front);
}