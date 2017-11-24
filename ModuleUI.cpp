#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "EntityManager.h"

#include "ConfigurationLoader.h"
#include "Timer.h"

#include "ModuleUI.h"


ModuleUI::ModuleUI(bool active) : Module(active)
{}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Init()
{
	if (LoadConfigFromFile() == false)
	{
		LOG("UI: failed to initialise\n");
		return false;
	}
	else
		return true;
}

UpdateStatus ModuleUI::Update(float)
{
	if (App->getInput().GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->getInput().GetGamepadButton(GamepadButton::START) == KEY_DOWN )
	{
		pause = !pause;
		if (pause)
			App->getTimer().TimerPause();
		else
			App->getTimer().TimerResume();
		if (App->getEntityManager().player != nullptr)
			App->getAudio().PlayFx(fx_pause);
	}
	if (pause == false)
	{
		if (remaining_msec_go_arrow > 0)
			remaining_msec_go_arrow -= App->getTimer().DeltaTime();

		if (App->getInput().GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
			debug = !debug;
	}

	PrintStatus();

	return UpdateStatus::Continue;
}
bool ModuleUI::CleanUp() {
	
	return true;
}

// -------------------------- Miscellaneous -------------------------------
void ModuleUI::PrintStatus()
{
	App->getRenderer().Blit(hud_graphics, 0, 0, &hud_section, 0.0F);
	if (pause)
	{
		App->getFonts().Print(112, 96, ModuleFonts::Fonts::scene_overlap, "P");
		App->getFonts().Print(132, 96, ModuleFonts::Fonts::scene_overlap, "A");
		App->getFonts().Print(152, 96, ModuleFonts::Fonts::scene_overlap, "U");
		App->getFonts().Print(172, 96, ModuleFonts::Fonts::scene_overlap, "S");
		App->getFonts().Print(192, 96, ModuleFonts::Fonts::scene_overlap, "E");
	}
	if (App->getEntityManager().player != nullptr) {
		App->getFonts().Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, App->getFonts().GetPrintableValue(App->getEntityManager().time_left_msec / 2000, 2));
		App->getFonts().Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, App->getFonts().GetPrintableValue(App->getEntityManager().player->score, 6));
		App->getFonts().Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, App->getFonts().GetPrintableValue(App->getEntityManager().player->help, 1));
		App->getFonts().Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, App->getFonts().GetPrintableValue(App->getEntityManager().player->lives, 1));
		PrintPlayerHealth();
		PrintGoArrow();
	}
	else {
		App->getFonts().Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, "00");
		App->getFonts().Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, "000000");
		App->getFonts().Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, "0");
		App->getFonts().Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, "0");
		App->getFonts().Print(146, 110, ModuleFonts::Fonts::scene_overlap, "END");
	}
	if (App->getEntityManager().boss != nullptr)
	{
		App->getRenderer().Blit(hud_graphics, hud_boss_pos.x, hud_boss_pos.y, &hud_boss_section, 0.0F);
		App->getFonts().Print(hud_boss_msg_pos.x, hud_boss_msg_pos.y, ModuleFonts::Fonts::hud_small, "-BOSS-");
		PrintBossHealth();
	}
	// debug options
	App->getFonts().Print(220, 2, ModuleFonts::Fonts::hud_small, "- TOOLS -");
	App->getFonts().Print(196, 11, ModuleFonts::Fonts::hud_small, "F1-COLS");
	App->getFonts().Print(196, 20, ModuleFonts::Fonts::hud_small, "F2-SPWN");
	App->getFonts().Print(260, 11, ModuleFonts::Fonts::hud_small, "F3-PLYR");
	App->getFonts().Print(260, 20, ModuleFonts::Fonts::hud_small, "F4-POS");

	//debug show
	if (debug)
	{
		App->getFonts().Print(112, 40, ModuleFonts::Fonts::hud_small, "X-");
		App->getFonts().Print(128, 40, ModuleFonts::Fonts::hud_small, App->getFonts().GetPrintableValue(App->getEntityManager().player->position.x, 4));
		App->getFonts().Print(168, 40, ModuleFonts::Fonts::hud_small, "Y-");
		App->getFonts().Print(184, 40, ModuleFonts::Fonts::hud_small, App->getFonts().GetPrintableValue(App->getEntityManager().player->position.y, 4));
	}

}


void ModuleUI::PrintPlayerHealth()
{
	int min_pixels = 2;
	int sections_to_draw = App->getEntityManager().player->health / min_pixels;
	for (int i = 0; i < sections_to_draw; ++i)
	{
		App->getRenderer().Blit(hud_graphics, hud_health_pos.x + i*hud_health_section.w, hud_health_pos.y, &hud_health_section, 0.0F);
	}
}

void ModuleUI::PrintBossHealth()
{
	int min_pixels = 2;
	int sections_to_draw;
	SDL_Rect *h_section;

	if (App->getEntityManager().boss->health > 110)
	{
		sections_to_draw = 40;
		h_section = &hud_high_health_section;
	}
	else if (App->getEntityManager().boss->health > 80)
	{
		sections_to_draw = 40;
		h_section = &hud_medium_health_section;
	}
	else
	{
		sections_to_draw = App->getEntityManager().boss->health / min_pixels;
		h_section = &hud_health_section;
	}
	for (int i = 0; i < sections_to_draw; ++i)
		App->getRenderer().Blit(hud_graphics, hud_health_boss_pos.x + i*hud_health_section.w, hud_health_boss_pos.y, h_section, 0.0F);
}

void ModuleUI::PrintGoArrow()
{
	static int audio_in_turn;
	if (remaining_msec_go_arrow <= 0)
		return;

	int current_turn = remaining_msec_go_arrow / blink_msec_go_arrow;
	bool show = current_turn % 2 == 0 ? true : false;

	if (show)
	{
		App->getRenderer().Blit(hud_graphics, hud_go_arrow_pos.x, hud_go_arrow_pos.y, &hud_go_arrow_section, 0.0F);
		if (audio_in_turn != current_turn)
		{
			App->getAudio().PlayFx(fx_go_arrow);
			audio_in_turn = current_turn;
		}
	}
}

void ModuleUI::ShowGoArrow()
{
	remaining_msec_go_arrow = 7 * blink_msec_go_arrow;
}

void ModuleUI::ShowCollidersDebugMode()
{
	if (active == true)
	{
		App->getFonts().Print(8, 40, ModuleFonts::Fonts::hud_small, "COLS ON");
	}
}

void ModuleUI::ShowEntityManagerDebugMode()
{
	if (active == true)
	{
		App->getFonts().Print(8, 48, ModuleFonts::Fonts::hud_small, "SPWN ON");
	}
}

void ModuleUI::ShowPlayerDebugMode()
{
	if (active == true)
	{
		App->getFonts().Print(8, 56, ModuleFonts::Fonts::hud_small, "PLYR ON");
	}
}



bool ModuleUI::LoadConfigFromFile()
{
	JSON_Object *ui_object = App->getConfig().GetJSONObject("hud"); 
	if (ui_object == nullptr) { return false;  }
	
	hud_graphics = App->getTextures().Load(App->getConfig().GetStringFromJSONObject(ui_object, "graphics_file"));
	if (hud_graphics == nullptr) { return false; }

	if (App->getConfig().LoadSDLRectFromJSONObject(ui_object, "section", &hud_section) == false) { return false; }
	if (App->getConfig().LoadSDLRectFromJSONObject(ui_object, "health_section", &hud_health_section) == false) { return false; }
	if (App->getConfig().LoadSDLRectFromJSONObject(ui_object, "boss_section", &hud_boss_section) == false) { return false; }
	if (App->getConfig().LoadSDLRectFromJSONObject(ui_object, "high_health_section", &hud_high_health_section) == false) { return false; }
	if (App->getConfig().LoadSDLRectFromJSONObject(ui_object, "medium_health_section", &hud_medium_health_section) == false) { return false; }
	if (App->getConfig().LoadSDLRectFromJSONObject(ui_object, "go_arrow_section", &hud_go_arrow_section) == false) { return false; }

	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "time_pos", &hud_time_pos) == false) { return false;  }
	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "score_pos", &hud_score_pos) == false) { return false; }
	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "lives_pos", &hud_lives_pos) == false) { return false; }
	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "help_pos", &hud_help_pos) == false) { return false; }
	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "health_pos", &hud_health_pos) == false) { return false; }
	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "boss_pos", &hud_boss_pos) == false) { return false; }
	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "boss_health_pos", &hud_health_boss_pos) == false) { return false; }
	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "boss_msg_pos", &hud_boss_msg_pos) == false) { return false; }
	if (App->getConfig().LoadiPointFromJSONObject(ui_object, "go_arrow_pos", &hud_go_arrow_pos) == false) { return false; }

	blink_msec_go_arrow = App->getConfig().GetIntFromJSONObject(ui_object, "go_arrow_blink_msec"); 
	if (blink_msec_go_arrow == 0) { return false;  }
	
	ui_object = App->getConfig().GetJSONObject("fx"); 
	if (ui_object == nullptr) { return false; }
	fx_pause = App->getAudio().LoadFx(App->getConfig().GetStringFromJSONObject(ui_object, "pause"));
	if (fx_pause == -1) { return false; }
	fx_go_arrow = App->getAudio().LoadFx(App->getConfig().GetStringFromJSONObject(ui_object, "go_arrow"));
	if (fx_go_arrow == -1) { return false; }

	return true;
}