#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleSceneSpace.h"
#include "ModuleFadeToBlack.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneSpace::ModuleSceneSpace(bool active) : Module(active)
{}

ModuleSceneSpace::~ModuleSceneSpace()
{}

// Load assets
bool ModuleSceneSpace::Start()
{
	LOG("Loading space scene");
	
	background = App->textures->Load("rtype/background.png");

	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();

	App->audio->PlayMusic("rtype/stage1.ogg", 1.0f);
	
	// TODO 15: create some colliders for the walls
	// solution wall coords: {0, 224, 3930, 16} {1375, 0, 111, 96} {1375, 145, 111, 96}
	//App->collision->AddCollider({ SCREEN_WIDTH,0,10,SCREEN_HEIGHT },colliderType::WALLS);	// test one :)
	App->collision->AddCollider({ 0, 224, 3930, 16 }, colliderType::WALLS);
	App->collision->AddCollider({ 1375, 0, 111, 96 }, colliderType::WALLS);
	App->collision->AddCollider({ 1375, 145, 111, 96 }, colliderType::WALLS);

	return true;
}

// UnLoad assets	
bool ModuleSceneSpace::CleanUp()
{
	LOG("Unloading space scene");

 	App->textures->Unload(background);
	App->player->Disable();
	App->collision->Disable();
	App->particles->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneSpace::Update()
{
	// Move camera forward -----------------------------
	int scroll_speed = 1;

	App->player->position.x += scroll_speed;
	App->player->collider->rect.x += scroll_speed;	// MENOS MAL!
	App->renderer->camera.x -= 3;
	
	// Draw everything --------------------------------------
	App->renderer->Blit(background, 0, 0, NULL);

	return UPDATE_CONTINUE;
}