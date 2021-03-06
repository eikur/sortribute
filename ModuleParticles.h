#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"

struct SDL_Texture;

struct Particle
{
	bool to_delete = false;

	unsigned int fx;
	iPoint		position;
	Animation*	animation;
	
	int speed;
	int time_to_live;
	int age;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	UpdateStatus Update(float dt = 0.0f) override;
	bool CleanUp();

	void AddParticle(const Particle& particle, int x, int y); // feel free to expand this call

private:
	SDL_Texture* graphics = nullptr;
	std::list<Particle*> active;

public:

	// prototype particles go here ...
	Particle* proto_laser;
	Particle* proto_explosion;
};

#endif // __MODULEPARTICLES_H__