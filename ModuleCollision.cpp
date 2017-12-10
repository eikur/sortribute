
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleUI.h"

using namespace std;

ModuleCollision::ModuleCollision(Module& entitiesReporter, Module& sceneReporter) : entitiesReporter(entitiesReporter), scenesReporter(sceneReporter)
{
	
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

UpdateStatus ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (auto&& it = _colliders.begin(); it != _colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			it->reset();
			it = _colliders.erase(it);
		}
		else
		{
			++it;
		}
	}

	return UpdateStatus::Continue;
}

UpdateStatus ModuleCollision::Update(float)
{
	for (auto& c1 : _colliders)
	{
		for (auto& c2 : _colliders)	
		{
			if (c1->CheckCollision(c2->getRect()) == true && c1 != c2)
			{
				if (collision_matrix[c1->type][c2->type] == 1)
				{
					if (c1->type != colliderType::SCENE_TRIGGER && c2->type != colliderType::SCENE_TRIGGER )
						entitiesReporter.HandleCollision(c1.get(), c2.get());
					else
						scenesReporter.HandleCollision(c1.get(), c2.get());
					
				}
			}
		}
	}

	if (App->getUI().pause == false)
	{
		if (App->getInput().GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			debug = !debug;
	}
	if (debug == true)
	{
		DebugDraw();
		App->getUI().ShowCollidersDebugMode();
	}
	return UpdateStatus::Continue;
}

void ModuleCollision::DebugDraw()
{
	for (auto& collider : _colliders)
	{
		if (!collider->isActive())
		{
			continue;
		}
		if (collider->type == colliderType::PLAYER_ATTACK)
			App->getRenderer().DrawQuad(collider->getRect(), 0, 0, 255, 120);
		else if (collider->type == colliderType::PLAYER)
			App->getRenderer().DrawQuad(collider->getRect(), 0, 0, 255, 80);
		else if (collider->type == colliderType::ENEMY)
			App->getRenderer().DrawQuad(collider->getRect(), 255, 0, 0, 80);
		else if (collider->type == colliderType::ENEMY_ATTACK)
			App->getRenderer().DrawQuad(collider->getRect(), 255, 0, 0, 120);
		else if (collider->type == colliderType::ITEMS)
			App->getRenderer().DrawQuad(collider->getRect(), 0, 255, 0, 80);
		else 
			App->getRenderer().DrawQuad(collider->getRect(), 255, 0, 255, 40);

	}
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (auto&& it : _colliders)
	{
		it.reset();
	}

	_colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, colliderType type, Entity* parent)
{
	_colliders.push_back(std::make_unique<Collider>(rect, type, parent));
	return _colliders.back().get();
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (_active == false)
	{
		return false;
	}
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
