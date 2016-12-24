#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("rtype/particles.png");

	// TODO 2: Create a prototype for the laser particle
	// audio: rtype/laser.wav
	// coords: {232, 103, 16, 12}; {249, 103, 16, 12};
	proto_laser = new Particle();
	proto_laser->fx = App->audio->LoadFx("rtype/laser.wav");
	proto_laser->animation->frames.push_back({ 232, 103, 16, 12 });
	proto_laser->animation->frames.push_back({ 249, 103, 16, 12 });
	proto_laser->animation->speed = 0.40f;
	proto_laser->speed = 3;
	proto_laser->time_to_live = 100;
	proto_laser->age = 0;
	proto_laser->has_collider = true;
	proto_laser->collider = new Collider({ 0,0,16,12 }, colliderType::PLAYER_SHOT);

	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};
	proto_explosion = new Particle();
	proto_explosion->fx = App->audio->LoadFx("rtype/explosion.wav");
	proto_explosion->animation->frames.push_back({ 274, 296, 33, 30 });
	proto_explosion->animation->frames.push_back({ 313, 296, 33, 30 });
	proto_explosion->animation->frames.push_back({ 346, 296, 33, 30 });
	proto_explosion->animation->frames.push_back({ 382, 296, 33, 30 });
	proto_explosion->animation->frames.push_back({ 419, 296, 33, 30 });
	proto_explosion->animation->frames.push_back({ 457, 296, 33, 30 });
	proto_explosion->animation->speed = 0.20f;
	proto_explosion->speed = 0;
	proto_explosion->time_to_live = 30;
	proto_explosion->age = 0;
	proto_explosion->has_collider = false;


	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// Update: draw particles 
update_status ModuleParticles::Update()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		Particle* p = *it;

		if(p->Update() == false)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
		{
			App->renderer->Blit(graphics, (*it)->position.x, (*it)->position.y, &(*it)->animation->GetCurrentFrame(), 1.0f);
			++it;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y)
{
	// TODO 4: Fill in a method to create an instance of a prototype particle
	Particle* new_particle = new Particle(particle);
	new_particle->position.x = x;
	new_particle->position.y = y;

	if (new_particle->has_collider)
	{
		new_particle->collider = App->collision->AddCollider({ x,y,particle.collider->rect.w , particle.collider->rect.h }, colliderType::PLAYER_SHOT);
	}
	// reproducir el sonido 
	App->audio->PlayFx(particle.fx, 0);
	active.push_back(new_particle);
	

}


// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	animation = new Animation();
	collider = new Collider();
}

// TODO 3: Fill in a copy constructor

Particle::Particle(const Particle& p)
{
	animation = new Animation(*p.animation);
	fx = p.fx;
	time_to_live = p.time_to_live;
	age = p.age;
	speed = p.speed;
	has_collider = p.has_collider;
	if (has_collider)
		collider = p.collider;
	
}

Particle::~Particle()
{
}

bool Particle::Update()
{
	// TODO 5: This is the core of the particle functionality.
	// Return false if the particle must be destroyed
	//bool ret = true;

	//death by age
	++age;
	if(age >= time_to_live)	{
		to_delete = true;
		if (has_collider)
			collider->to_delete = true;
		return false;
	}
	//death by collision
	if (has_collider && collider->to_delete == true)
	{
		to_delete = true;
		return false;
	}
	//else
	position.x += speed;
	if (has_collider)
		collider->rect.x += speed;

	return true;
}
