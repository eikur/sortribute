
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleUI.h"
#include "Timer.h"

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
		if ((*it)->hasToBeDeleted())
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
				if (collision_matrix[c1->getType()][c2->getType()] == 1)
				{
					if (c1->getType() != colliderType::SCENE_TRIGGER && c2->getType() != colliderType::SCENE_TRIGGER )
						entitiesReporter.HandleCollision(c1.get(), c2.get());
					else
						scenesReporter.HandleCollision(c1.get(), c2.get());
					c1->safeCallback(c2->getType());
					c2->safeCallback(c1->getType());
				}
			}
		}
	}

	if (App->getTimer().isRunning())
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
		if (collider->getType() == colliderType::PLAYER_ATTACK)
			App->getRenderer().DrawQuad(collider->getRect(), 0, 0, 255, 120);
		else if (collider->getType() == colliderType::PLAYER)
			App->getRenderer().DrawQuad(collider->getRect(), 0, 0, 255, 80);
		else if (collider->getType() == colliderType::ENEMY)
			App->getRenderer().DrawQuad(collider->getRect(), 255, 0, 0, 80);
		else if (collider->getType() == colliderType::ENEMY_ATTACK)
			App->getRenderer().DrawQuad(collider->getRect(), 255, 0, 0, 120);
		else if (collider->getType() == colliderType::ITEMS)
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

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, colliderType type, Entity* parent, const CollisionCallback& callback)
{
	_colliders.push_back(std::make_unique<Collider>(rect, type, parent, callback));
	return _colliders.back().get();
}

// -----------------------------------------------------
colliderType Collider::getType() const
{
	return _type;
}

Entity* Collider::getParent() const
{
	return _parent;
}


void Collider::setPos(iPoint pos)
{
	rect.x = pos.x;
	rect.y = pos.y;
}

void Collider::SetRect(const SDL_Rect& r)
{
	rect.x = r.x;
	rect.y = r.y;
	rect.h = r.h;
	rect.w = r.w;
}
const SDL_Rect& Collider::getRect() const
{
	return rect;
}

void Collider::setActive(bool value)
{
	_active = value;
}

bool Collider::isActive() const
{
	return _active;
}

bool Collider::hasCallback() const
{
	return _callback != nullptr;
}

void Collider::safeCallback(colliderType param)
{
	if (hasCallback())
	{
		_callback(param);
	}
}

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

bool Collider::hasToBeDeleted() const
{
	return _toBeDeleted;
}

void Collider::setToBeDeleted()
{
	_toBeDeleted = true;
}