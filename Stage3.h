#pragma once

#include "Animation.h"
#include "Scene.h"
#include "Point.h"

struct SDL_Texture;
class Player;

class Stage3 : public Scene 
{
public:
	Stage3(SceneManager& manager, EntityManager& entityManager);

	bool Init() override;
	bool Start() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update(float dt = 0.0f) override;
	bool CleanUp() override;

	void HandleCollision(Collider* a, Collider* b) override;

private:
	bool LoadConfigFromFile(const char* file_path);
	void CreateSceneTriggers();
	void DeleteSceneTriggers();
	void TriggerCollisionManagement(Collider *trigger);
	void PlaceSceneItems();

public:
	Player* player;

private:
	SDL_Texture* graphics = nullptr;
	
	SDL_Rect background_section;
	iPoint background_pos = { 0,0 };

	SDL_Rect middleground_section;
	iPoint middleground_pos = { 0,0 };

	SDL_Rect foreground_section;
	iPoint foreground_pos = {0, 0};

	Animation wave_sand;
	iPoint wave_sand_pos = { 0, 0 };

	Animation wave_splash;
	iPoint wave_splash_pos = { 0, 0 };

	std::string music_path = "";
	std::string boss_music_path = "";
	unsigned int fx_waves = 0;

	// scene triggers
	std::vector<Collider*> triggers;

	Collider *spawn1 = nullptr;
	Collider *spawn2 = nullptr;
	Collider *spawn3 = nullptr;
	Collider *spawn4 = nullptr;
	Collider *spawn5 = nullptr;
	Collider *spawn6 = nullptr;
	Collider *spawn7 = nullptr;
	Collider *spawn8 = nullptr;
	Collider *spawn9 = nullptr;
	Collider *spawn10 = nullptr;

	Collider *cam_lock1 = nullptr;
	Collider *cam_lock2 = nullptr;
	Collider *cam_lock3 = nullptr;
	Collider *cam_lock4 = nullptr;
	
	Collider *battle_zone1 = nullptr;
	Collider *battle_zone2 = nullptr;
	Collider *battle_zone3 = nullptr;
	Collider *battle_zone4 = nullptr;
};
