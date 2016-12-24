#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");



	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int speed = 1;

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		collider->rect.x -= speed;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		collider->rect.x += speed;
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;
		collider->rect.y += speed;
		if(current_animation != &down)
		{
			down.Reset();
			current_animation = &down;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
		collider->rect.y -= speed;
		if(current_animation != &up)
		{
			up.Reset();
			current_animation = &up;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// TODO 6: Shoot a laser using the particle system
		App->particles->AddParticle(*(App->particles->proto_laser), App->player->position.x + 32, App->player->position.y );
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
	   && App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
		current_animation = &idle;

	// Draw everything --------------------------------------

   	if(destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));
	return UPDATE_CONTINUE;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position
void ModulePlayer::HandleCollision(Collider* first, Collider* second)
{
	if (first->type == colliderType::PLAYER_SHOT)
	{
		first->to_delete = true;
		App->particles->AddParticle(*(App->particles->proto_explosion), first->rect.x,first->rect.y-8);
	}
	if (second->type == colliderType::PLAYER_SHOT)
	{
		second->to_delete = true;
		App->particles->AddParticle(*(App->particles->proto_explosion), second->rect.x, second->rect.y-8);
	}

	if (first->type == colliderType::PLAYER || second->type == colliderType::PLAYER)
	{
		App->player->destroyed = true;
		App->player->collider->to_delete = true;
		App->particles->AddParticle(*(App->particles->proto_explosion), first->rect.x, first->rect.y);
		App->particles->AddParticle(*(App->particles->proto_explosion), first->rect.x, first->rect.y - 16);
	
		App->fade->FadeToBlack((Module*)App->scene_intro, (Module*)App->scene_space, 1.0f);
	}

}

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player

