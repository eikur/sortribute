#pragma once

#include "Module.h"
#include "Point.h"

struct SDL_Texture;

class ModuleUI : public Module
{
public:
	ModuleUI(bool active = true);
	~ModuleUI();

	bool Init();
	UpdateStatus Update(float dt = 0.0f) override;
	bool CleanUp();

	void ShowGoArrow();
	void ShowCollidersDebugMode();
	void ShowEntityManagerDebugMode();
	void ShowPlayerDebugMode();

private:
	void PrintStatus();
	void PrintPlayerHealth();
	void PrintBossHealth();
	void PrintGoArrow();

	bool LoadConfigFromFile();

public:
	bool pause = false;

private:
	unsigned int fx_pause = 0;

	//hud
	SDL_Texture *hud_graphics = nullptr;
	SDL_Rect hud_section = { 0,0,0,0 };
	iPoint hud_score_pos = { 0,0 };
	iPoint hud_health_pos = { 0,0 };
	SDL_Rect hud_health_section = { 0,0,0,0 };
	iPoint hud_time_pos = { 0,0 };
	iPoint hud_help_pos = { 0,0 };
	iPoint hud_lives_pos = { 0,0 };
	SDL_Rect hud_go_arrow_section = { 0,0,0,0 };
	iPoint hud_go_arrow_pos = { 0,0 };

	SDL_Rect hud_boss_section = { 0,0,0,0 };
	SDL_Rect hud_high_health_section = { 0,0,0,0 };
	SDL_Rect hud_medium_health_section = { 0,0,0,0 };
	iPoint hud_boss_pos = { 0,0 };
	iPoint hud_boss_msg_pos = { 0,0 };
	iPoint hud_health_boss_pos = { 0,0 };

	// go arrow
	float remaining_msec_go_arrow = 0.0f;
	float blink_msec_go_arrow = 800.0f;
	unsigned int fx_go_arrow = 0;

	// debug
	bool show_debug_help = false;
	bool debug = false;

};
