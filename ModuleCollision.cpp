
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

using namespace std;

ModuleCollision::ModuleCollision(Module* report_to) : report_to(report_to)
{
	
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	// TODO 8: Check collisions between all colliders. 
	// After making it work, review that you are doing the minumum checks possible
	for (list<Collider*>::const_iterator it = colliders.cbegin(); it != colliders.cend();++it )
	{
		for (list<Collider*>::const_iterator it2 = it; it2 != colliders.cend(); ++it2)	
		{
			if ((*it)->CheckCollision((*it2)->rect) == true)
			{
				if (collision_matrix[(*it)->type][(*it2)->type] == 1)
				{
					LOG("Collided");
					//report_to->HandleCollision((*it), (*it2));
					
				}
			}
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, colliderType type)
{
	Collider* ret = new Collider(rect, type);

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	// TODO 7: Create by hand (avoid consulting the internet) a simple collision test
	// Return true if the argument and the own rectangle are intersecting
	bool collision;
	if ((r.y + r.h <= rect.y))
		collision = false;
	else if (r.y >= (rect.y + rect.h))
		collision = false;
	else if ((r.x + r.w) <= rect.x)
		collision = false;
	else if (r.x >= (rect.x + rect.w))
		collision = false;
	else
		collision = true;
	return collision;

}
