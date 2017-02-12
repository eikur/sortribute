#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "EntityManager.h"

#include "Timer.h"

#include "ModuleUI.h"


ModuleUI::ModuleUI(bool active) : Module(active)
{}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Init()
{
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("UI: failed to initialise\n");
		return false;
	}
	else
		return true;
}

update_status ModuleUI::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		pause = !pause;
		if (pause)
			App->timer->TimerPause();
		else
			App->timer->TimerResume();
		if (App->manager->player != nullptr)
			App->audio->PlayFx(fx_pause);
	}
	if (pause == false)
	{
		if (remaining_msec_go_arrow > 0)
			remaining_msec_go_arrow -= App->timer->DeltaTime();

		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
			debug = !debug;
	}

	PrintStatus();

	return UPDATE_CONTINUE;
}
bool ModuleUI::CleanUp() {
	App->textures->Unload(hud_graphics);
	return true;
}

// -------------------------- Miscellaneous -------------------------------
void ModuleUI::PrintStatus()
{
	App->renderer->Blit(hud_graphics, 0, 0, &hud_section, 0.0F);
	if (pause)
	{
		App->fonts->Print(102, 96, ModuleFonts::Fonts::scene_overlap, "PAUSE");
	}
	if (App->manager->player != nullptr) {
		App->fonts->Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(App->manager->time_left_msec / 2000, 2));
		App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, App->fonts->GetPrintableValue(App->manager->player->score, 6));
		App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(App->manager->player->help, 1));
		App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(App->manager->player->lives, 1));
		PrintPlayerHealth();
		PrintGoArrow();
	}
	else {
		App->fonts->Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, "00");
		App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, "000000");
		App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, "0");
		App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, "0");
		App->fonts->Print(146, 110, ModuleFonts::Fonts::scene_overlap, "END");
	}
	if (App->manager->boss != nullptr)
	{
		App->renderer->Blit(hud_graphics, hud_boss_pos.x, hud_boss_pos.y, &hud_boss_section, 0.0F);
		App->fonts->Print(hud_boss_msg_pos.x, hud_boss_msg_pos.y, ModuleFonts::Fonts::hud_small, "-BOSS-");
		PrintBossHealth();
	}
	// debug options
	App->fonts->Print(184, 6, ModuleFonts::Fonts::hud_small, "F1-COLS");
	App->fonts->Print(184, 16, ModuleFonts::Fonts::hud_small, "F2-SPWN");
	App->fonts->Print(256, 6, ModuleFonts::Fonts::hud_small, "F3-PLYR");
	App->fonts->Print(256, 16, ModuleFonts::Fonts::hud_small, "F4-POS");

	//debug show
	if (debug)
	{
		App->fonts->Print(112, 40, ModuleFonts::Fonts::hud_small, "X-");
		App->fonts->Print(128, 40, ModuleFonts::Fonts::hud_small, App->fonts->GetPrintableValue(App->manager->player->position.x, 4));
		App->fonts->Print(168, 40, ModuleFonts::Fonts::hud_small, "Y-");
		App->fonts->Print(184, 40, ModuleFonts::Fonts::hud_small, App->fonts->GetPrintableValue(App->manager->player->position.y, 4));
	}

}


void ModuleUI::PrintPlayerHealth()
{
	int min_pixels = 2;
	int sections_to_draw = App->manager->player->health / min_pixels;
	for (int i = 0; i < sections_to_draw; ++i)
	{
		App->renderer->Blit(hud_graphics, hud_health_pos.x + i*hud_health_section.w, hud_health_pos.y, &hud_health_section, 0.0F);
	}
}

void ModuleUI::PrintBossHealth()
{
	int min_pixels = 2;
	int sections_to_draw;
	SDL_Rect *h_section;

	if (App->manager->boss->health > 110)
	{
		sections_to_draw = 40;
		h_section = &hud_high_health_section;
	}
	else if (App->manager->boss->health > 80)
	{
		sections_to_draw = 40;
		h_section = &hud_medium_health_section;
	}
	else
	{
		sections_to_draw = App->manager->boss->health / min_pixels;
		h_section = &hud_health_section;
	}
	for (int i = 0; i < sections_to_draw; ++i)
		App->renderer->Blit(hud_graphics, hud_health_boss_pos.x + i*hud_health_section.w, hud_health_boss_pos.y, h_section, 0.0F);
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
		App->renderer->Blit(hud_graphics, hud_go_arrow_pos.x, hud_go_arrow_pos.y, &hud_go_arrow_section, 0.0F);
		if (audio_in_turn != current_turn)
		{
			App->audio->PlayFx(fx_go_arrow);
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
	if (pause == false)
	{
		App->fonts->Print(8, 40, ModuleFonts::Fonts::hud_small, "COLS ON");
	}
}

void ModuleUI::ShowEntityManagerDebugMode()
{
	if (pause == false)
	{
		App->fonts->Print(8, 48, ModuleFonts::Fonts::hud_small, "SPWN ON");
	}
}

void ModuleUI::ShowPlayerDebugMode()
{
	if (pause == false)
	{
		App->fonts->Print(8, 56, ModuleFonts::Fonts::hud_small, "PLYR ON");
	}
}



bool ModuleUI::LoadConfigFromFile(const char *file_path)
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

	j_array = json_object_dotget_array(root_object, "hud.boss_section");
	hud_boss_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.high_health_section");
	hud_high_health_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.medium_health_section");
	hud_medium_health_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.boss_pos");
	hud_boss_pos.x = (int)json_array_get_number(j_array, 0);
	hud_boss_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.boss_health_pos");
	hud_health_boss_pos.x = (int)json_array_get_number(j_array, 0);
	hud_health_boss_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.boss_msg_pos");
	hud_boss_msg_pos.x = (int)json_array_get_number(j_array, 0);
	hud_boss_msg_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	if (json_object_dothas_value_of_type(root_object, "fx.pause", JSONString))
		fx_pause = App->audio->LoadFx(json_object_dotget_string(root_object, "fx.pause"));

	j_array = json_object_dotget_array(root_object, "hud.go_arrow_pos");
	hud_go_arrow_pos.x = (int)json_array_get_number(j_array, 0);
	hud_go_arrow_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.go_arrow_section");
	hud_go_arrow_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	blink_msec_go_arrow = (int)json_object_dotget_number(root_object, "hud.go_arrow_blink_msec");

	if (json_object_dothas_value_of_type(root_object, "hud.fx_go_arrow", JSONString))
		fx_go_arrow = App->audio->LoadFx(json_object_dotget_string(root_object, "hud.fx_go_arrow"));

	json_value_free(root_value);

	return true;
}