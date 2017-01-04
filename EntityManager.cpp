#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleTimer.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

#include "Player.h"
#include "EnemyGarcia.h"

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

update_status EntityManager::Update() 
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		pause = !pause;
		if (pause)
			App->timer->TimerPause();
		else
			App->timer->TimerResume();
	}

	if (pause)
	{
		
	}
	else
	{
		elapsed_msec += App->timer->DeltaTime();

		if (elapsed_msec >= upd_logic_msec)
			upd_logic = true;

		
		entities.sort(Entity::ptrEntityDepthComparison());
		for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end();)
		{
			if ((*it)->Update(elapsed_msec, upd_logic) == false)
			{

				if (*it == player)
					player = nullptr;
				else
					player->AddScore((*it)->score);
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

	PrintStatus();
	CheatCodes();

	return UPDATE_CONTINUE;
}

bool EntityManager::CleanUp()
{
	LOG("EntityManager: Removing entities from application\n");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;
	entities.clear();

	return true;
}

//---------------------- Entity Management ------------------
Entity* EntityManager::CreateEntity(Entity::Types type)
{
	static_assert(Entity::Types::unknown == 2, "code needs update");
	Entity* ret = nullptr;

	switch (type)
	{
	case Entity::Types::player: ret = new Player(this); break;
	case Entity::Types::npc_garcia: ret = new EnemyGarcia(this); break;
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

void EntityManager::HandleCollision(Collider* a, Collider* b)
{
	Entity* first = nullptr;
	Entity* second = nullptr;
	colliderType first_type;
	colliderType second_type;
		
	// order by type to ease the logic
	if (a->type <= b->type)
	{
		first = a->parent; first_type = a->type; 
		second = b->parent; second_type = b->type;
	}
	else
	{
		first = b->parent; first_type = b->type; 
		second = a->parent; second_type = a->type;
	}

	int depth_difference = first->GetDepth() - second->GetDepth();
	if (depth_difference < -5 || depth_difference > 5)
		return;


	switch (first_type)
	{
		case colliderType::PLAYER:
			if (second_type == colliderType::ITEMS)	//Take item!
			{

			}
			else if (second_type == colliderType::ENEMY)	// Holding!
			{
				if ( first->IsGrounded() &&
					(first->facing_right == true && first->position.x <= second->position.x) ||
					(first->facing_right == false && second->position.x <= first->position.x) )
				{
					if (first->facing_right == second->facing_right)	// back hold
					{
						first->SetHoldingBack(); 
						second->SetBeingHoldBack();
					}
					else
					{
						first->SetHoldingFront();
						second->SetBeingHoldFront();
					}
					
					if (depth_difference != 0) {
						second->SetDepth(first->GetDepth());
					}
						
				}
				else 
				{
					second->SetIdle();
				}
			}
			else if (second_type == colliderType::ENEMY_ATTACK)	// Being hit!
			{
				if (second->attacking && first->hittable)
				{
					first->SetBeingHit();
					App->audio->PlayFx(second->fx_attack_hit);
					first->DecreaseHealth(8);	
				}
			}
			else
			{
				LOG("Bad defined collision, check collision matrix");
			}
		break;

		case colliderType::PLAYER_ATTACK:
			if (second_type == colliderType::ENEMY)
			{
				if (first->attacking  && second->hittable )	
				{
					second->SetBeingHit();
					App->audio->PlayFx(first->fx_attack_hit);	
					second->DecreaseHealth(8); // tmp
				}
			}
			else
			{
				LOG("Bad defined collision, check collision matrix");
			}
			break;

		default: 
			LOG("Bad defined collision, check collision matrix");
			break;
	}
}

// -------------------------- Miscellaneous -------------------------------
void EntityManager::PrintStatus()
{
	App->renderer->Blit(hud_graphics, 0, 0, &hud_section, 0.0F);
	App->fonts->Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(time_left, 2));
	if (player != nullptr){
		App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, App->fonts->GetPrintableValue(player->score, 6));
		App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(player->help, 1));
		App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(player->lives, 1));
		PrintPlayerHealth();
	}
	else {
		App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, "000000");
		App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, "0" );
		App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, "0");
		App->fonts->Print(146, 110, ModuleFonts::Fonts::scene_overlap, "END");
	}

}

void EntityManager::PrintPlayerHealth()
{
	int max_pixels = 80;
	int min_pixels = 2;
	int sections_to_draw = player->health / min_pixels;
	for (int i = 0; i < sections_to_draw; ++i)
	{
		App->renderer->Blit(hud_graphics, hud_health_pos.x + i*hud_health_section.w, hud_health_pos.y, &hud_health_section, 0.0F);
	}
}



bool EntityManager::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;
	JSON_Array *j_array;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	// hud load
	if (json_object_dothas_value_of_type(root_object, "hud.graphics_file", JSONString))
		hud_graphics = App->textures->Load(json_object_dotget_string(root_object, "hud.graphics_file"));
	if (hud_graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

	j_array = json_object_dotget_array(root_object, "hud.section");
	hud_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.time_pos");
	hud_time_pos.x = (int)json_array_get_number(j_array, 0);
	hud_time_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.score_pos");
	hud_score_pos.x = (int)json_array_get_number(j_array, 0);
	hud_score_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.lives_pos");
	hud_lives_pos.x = (int)json_array_get_number(j_array, 0);
	hud_lives_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.help_pos");
	hud_help_pos.x = (int)json_array_get_number(j_array, 0);
	hud_help_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.health_pos");
	hud_health_pos.x = (int)json_array_get_number(j_array, 0);
	hud_health_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.health_section");
	hud_health_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	json_value_free(root_value);
	return true;
}

void EntityManager::CheatCodes()
{
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && player != nullptr)
	{
		Entity *a = (Entity*) CreateEntity(Entity::Types::npc_garcia);
		if (a != nullptr){
			a->position.y = player->position.y;	
			a->position.x = player->position.x + 150;
			a->UpdatePosition({ 0,0 });
		}
	}
}



