#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "TextureHelper.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFonts.h"

//#include "3rdparty/SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	//graphics = App->getTextures().Load("rtype/particles.png");
	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
//	App->getTextures().Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// Update: draw particles 
UpdateStatus ModuleParticles::Update(float)
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
			App->getRenderer().Blit(graphics, (*it)->position.x, (*it)->position.y, &(*it)->animation->GetCurrentFrame(), 1.0f);
			++it;
		}
	}

	return UpdateStatus::Continue;
}

void ModuleParticles::AddParticle(const Particle&, int, int)
{
	// Method to create an instance of a prototype particle
}


// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	animation = new Animation();
	collider = new Collider();
}

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
	// Return false if the particle must be destroyed

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
