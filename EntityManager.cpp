#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "TextureHelper.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModuleUI.h"
#include "SceneManager.h"
#include "Timer.h"

#include "Player.h"
#include "EnemyGarcia.h"
#include "Apple.h"
#include "Chicken.h"
#include "Boss3.h"

#include "EntityManager.h"

EntityManager::EntityManager(bool active): Module(active){

}

EntityManager::~EntityManager() {
}

// ----- Module basic functions --------
bool EntityManager::Init()
{
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Entity Manager: failed to initialise\n");
		return false;
	}
	else 
		return true;
}

bool EntityManager::Start()
{
	return true;
}

UpdateStatus EntityManager::Update(float dt)
{
	if (App->getUI().pause == false)
	{
		elapsed_msec += dt;
		time_left_msec -= dt;

		if (time_left_msec <= 0 && player != nullptr)
		{
			if (player->IsAlive())
				player->TimeOver();
			time_left_msec = 0;
		}

		if (elapsed_msec >= upd_logic_msec)
			upd_logic = true;

		entities.sort(Entity::ptrEntityDepthComparison());
		unsigned int msecInt = static_cast<uint>(elapsed_msec);
		for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end();)
		{
			if ((*it)->Update(msecInt, upd_logic) == false)
			{
 				if (*it == player)
					player = nullptr;
				else
					player->AddScore((*it)->score);
				if (*it == boss)
					boss = nullptr;
				RELEASE(*it);
				it = entities.erase(it);
			}
			else
			{
				(*it)->Draw();
				++it;
			}
		}
		
		if (upd_logic == true)
		{
			elapsed_msec = 0;
			upd_logic = false;
		}
	} 

	if (player == nullptr)
	{
		App->getSceneManager().SwapScene(SceneManager::SceneId::Intro);
	}
	
	CheatCodes();

	return UpdateStatus::Continue;
}

bool EntityManager::CleanUp()
{
	LOG("EntityManager: Removing entities from application\n");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		(*it)->CleanUp(); (*it)->RemoveColliders();	delete *it;
	}
	entities.clear();
	enemy_queue.clear();

	return true;
}

//---------------------- Entity Management ------------------
Entity* EntityManager::CreateEntity(Entity::Types type)
{
	static_assert(Entity::Types::unknown == 5, "code needs update");
	Entity* ret = nullptr;

	switch (type)
	{
	case Entity::Types::player: ret = new Player(); break;
	case Entity::Types::npc_garcia: ret = new EnemyGarcia(player); break;
	case Entity::Types::item_apple: ret = new Apple(); break;
	case Entity::Types::item_chicken: ret = new Chicken(); break;
	case Entity::Types::npc_boss: ret = new Boss3(player); break;
	}

	if (ret != nullptr)
	{
		if (ret->Init() == false)
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			entities.push_back(ret);
		}
	}
	return ret;
}

void EntityManager::KnockDownAllEnemies(bool wipeout)  {
	for (std::list<Entity*>::const_iterator it = entities.cbegin(); it != entities.cend();++it)
	{
		if ((*it)->m_type == Entity::Types::npc_garcia || (*it)->m_type == Entity::Types::npc_boss)
		{
			if ((*it)->IsAlive())
			{
				if (wipeout)
					(*it)->SetBeingKnocked((*it)->max_health);
				else
					(*it)->SetBeingKnocked();
			}
		}
	}
}

int EntityManager::GetEnemyCount() {
	int ret = 0;
	for (std::list<Entity*>::const_iterator it = entities.cbegin(); it != entities.cend(); ++it)
		if ((*it)->m_type == Entity::Types::npc_garcia)
			ret += 1;
	return ret;
}

void EntityManager::RestoreTimeLeft()
{
	time_left_msec = 81999;
}

void EntityManager::HandleCollision(Collider* a, Collider* b)
{
	Entity* first = nullptr;
	Entity* second = nullptr;
	Collider* first_col = nullptr;
	Collider* second_col = nullptr;
		
	// order by type to ease the logic
	if (a->type <= b->type)
	{
		first = a->parent; first_col = a;
		second = b->parent; second_col = b;
	}
	else
	{
		first = b->parent; first_col = b;
		second = a->parent; second_col = a;
	}

	int depth_difference = first->GetDepth() - second->GetDepth();
	if (depth_difference < -layer_depth || depth_difference > layer_depth)
		return;


	switch (first_col->type)
	{
		case colliderType::PLAYER:
			if (second_col->type == colliderType::ITEMS)	
			{
				first->SetReachableItem(second);
			}
			else if (second_col->type == colliderType::ENEMY)
			{
				if (first->IsGrounded() && !first->IsHoldingSomeone() && first->AllowAnimationInterruption() &&
					second->IsGrounded() && !second->is_being_thrown_back && second->IsAlive() &&
					((first->facing_right == true && first_col->rect.x <= second_col->rect.x) ||
					(first->facing_right == false && second_col->rect.x <= first_col->rect.x)))
				{
					if (first->facing_right == second->facing_right)
					{
						first->SetHoldingBack(second); 
						second->SetBeingHoldBack();
					}
					else
					{
						first->SetHoldingFront(second);
						second->SetBeingHoldFront();
					}
					
					if (depth_difference != -1 || abs(first->position.x - second->position.x) < first_col->rect.w)
					{
						if (first->facing_right)
							second->UpdatePosition({ first->position.x - second->position.x + first_col->rect.w, first->position.y - second->position.y - 1 });
						else
							second->UpdatePosition({ first->position.x - second->position.x - first_col->rect.w, first->position.y - second->position.y - 1 });
					}
				}
			}
			else if (second_col->type == colliderType::ENEMY_ATTACK)
			{
				if (second->is_attacking && first->is_hittable && first->IsGrounded() && first->IsAlive())
				{
					if (second->position.x <= first->position.x)
						first->facing_right = false;
					else
						first->facing_right = true;

					App->getAudio().PlayFx(second->fx_attack_hit);
					if (second->current_combo_hits <= 2)
						first->SetBeingHit(second->attack1_dmg);
					else if (second->current_combo_hits == 3)
						first->SetBeingKnocked(second->attack2_dmg);
					second->combo_remaining_msec = second->combo_window_msec;

				}
			}
			else
			{
				LOG("Bad defined collision, check collision matrix");
			}
		break;

		case colliderType::PLAYER_ATTACK:
			if (second_col->type == colliderType::ENEMY)
			{
				if (first->is_attacking  && second->is_hittable && second->IsAlive() )	
				{
					second->is_hittable = false;
					App->getAudio().PlayFx(first->fx_attack_hit);
					if (first->position.x <= second->position.x)
						second->facing_right = false;
					else
						second->facing_right = true;

					if (first->IsGrounded())
					{
						if (first->current_combo_hits <= 2)
							second->SetBeingHit(first->attack1_dmg);
						else if (first->current_combo_hits <= 3)
							second->SetBeingHit(first->attack2_dmg);
						else if( first->current_combo_hits == 4)
						{					
							second->SetBeingKnocked(first->attack3_dmg);
							first->combo_remaining_msec = 0;
						}

						first->combo_remaining_msec = first->combo_window_msec;
					}
					else {
						second->SetBeingKnocked(first->attack3_dmg);
					}
				}
			}
			else
			{
				LOG("Bad defined collision, check collision matrix");
			}
			break;

		case colliderType::ENEMY:
			if (second_col->type == colliderType::ENEMY) 
			{
				if (first->is_being_thrown_front && first->IsGrounded() == false && second->is_hittable && second->IsAlive() )
					second->SetBeingKnocked(first->throw_dmg);
				else if (second->is_being_thrown_front && second->IsGrounded() == false && first->is_hittable && first->IsAlive())
					first->SetBeingKnocked(second->throw_dmg);
			}
			else
			{
				LOG("Bad defined collisions, check collision matrix");
			}
			break;
	}
}

bool EntityManager::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	// depth for collisions
	layer_depth = (int)json_object_dotget_number(root_object, "collision.layer_depth");

	json_value_free(root_value);

	return true;
}

void EntityManager::CheatCodes()
{
	if (debug)
		App->getUI().ShowEntityManagerDebugMode();

	if (App->getUI().pause == false)
	{
		if (App->getInput().GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			debug = !debug;
		if (debug)
		{
			if (App->getInput().GetKey(SDL_SCANCODE_E) == KEY_DOWN && player != nullptr)
			{
				Entity *a = (Entity*)CreateEntity(Entity::Types::npc_garcia);
				if (a != nullptr) {
					a->SetPosition({ player->position.x + 147, player->GetDepth() });
				}
			}
			if (App->getInput().GetKey(SDL_SCANCODE_R) == KEY_DOWN && player != nullptr)
			{
				Entity *a = (Entity*)CreateEntity(Entity::Types::npc_boss);
				if (a != nullptr) {
					a->SetPosition({ player->position.x + 147, player->GetDepth() });
					boss = a;
				}
			}
			if (App->getInput().GetKey(SDL_SCANCODE_A) == KEY_DOWN && player != nullptr)
			{
				Entity *a = (Entity*)CreateEntity(Entity::Types::item_apple);
				if (a != nullptr) {
					a->SetPosition({ player->position.x + 48, player->GetDepth() });

				}
				return;
			}
			if (App->getInput().GetKey(SDL_SCANCODE_S) == KEY_DOWN && player != nullptr)
			{
				Entity *a = (Entity*)CreateEntity(Entity::Types::item_chicken);
				if (a != nullptr) {
					a->SetPosition({ player->position.x + 48, player->GetDepth() });

				}
				return;
			}
			if (App->getInput().GetKey(SDL_SCANCODE_4) == KEY_REPEAT && player != nullptr)
			{
				time_left_msec -= 2000;
				return;
			}
		}
	}

	// enable player debug mode
	if (player != nullptr)
		player->CheatCodes();
}



