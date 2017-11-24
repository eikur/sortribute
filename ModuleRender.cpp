#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "EntityManager.h"
#include "ConfigurationLoader.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;

	if (LoadConfigFromFile() == false)
	{ 
		LOG("Renderer: Unable to load configuration from file\n");
		ret = false;
	}
	else
	{
		camera.x = camera.y = 0;
		camera.w = m_screen_width * m_screen_size;
		camera.h = m_screen_height* m_screen_size;

		Uint32 flags = 0;
		if (m_vsync == true)
		{
			flags |= SDL_RENDERER_PRESENTVSYNC;
		}

		renderer = SDL_CreateRenderer(App->getWindow().m_window, -1, flags);

		if (renderer == nullptr)
		{
			LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

UpdateStatus ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UpdateStatus::Continue;
}

// Called every draw update
UpdateStatus ModuleRender::Update()
{
	//move camera
	/*
	if (App->getInput().GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		locked = !locked;
	}

	if (App->getInput().GetKey(SDL_SCANCODE_O) == KEY_REPEAT)
		App->getRenderer().camera.x += m_speed;

	if (App->getInput().GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
		App->getRenderer().camera.x -= m_speed;
		*/
	return UpdateStatus::Continue;
}


UpdateStatus ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UpdateStatus::Continue;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, bool flip)
{
	bool ret = true;
	SDL_Rect rect;
	SDL_RendererFlip rflip = (flip == true)? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE;

	rect.x = (int)(camera.x * speed) + x * m_screen_size;
	rect.y = (int)(camera.y * speed) + y * m_screen_size;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= m_screen_size;
	rect.h *= m_screen_size;

	if (SDL_RenderCopyEx(renderer, texture, section, &rect,0,NULL,rflip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * m_screen_size);
		rec.y = (int)(camera.y + rect.y * m_screen_size);
		rec.w *= m_screen_size;
		rec.h *= m_screen_size;
	}

	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

//---------------------------------------------------------------

void ModuleRender::GetPlayerPositionLimits( SDL_Rect &player_limits ) const
{
	player_limits.x = (int) -camera.x/m_screen_size + m_limit_margin;	
}

void ModuleRender::MoveCamera(int x_pos, int x_speed) {
	if (locked == false)
	{
		int target_xpos_camera = (x_pos) * m_screen_size;	
		if ((-camera.x + camera.w / 2) < target_xpos_camera)
			camera.x -= x_speed*m_screen_size;
	}
}

bool ModuleRender::LoadConfigFromFile()
{
	JSON_Object *render_object = nullptr; 
	
	render_object = App->getConfig().GetJSONObject("window");
	if (render_object == nullptr) { return false; }
	m_screen_width = App->getConfig().GetIntFromJSONObject(render_object, "screen_width"); 
	if (m_screen_width == 0) { return false; }
	m_screen_height = App->getConfig().GetIntFromJSONObject(render_object, "screen_height");
	if (m_screen_height == 0) { return false; }
	m_screen_size = App->getConfig().GetIntFromJSONObject(render_object, "screen_size");
	if (m_screen_size == 0) { return false; }
	m_vsync = App->getConfig().GetBoolFromJSONObject(render_object, "vsync"); 

	render_object = App->getConfig().GetJSONObject("renderer.camera"); 
	if (render_object == nullptr) { return false; }
	m_limit_margin = App->getConfig().GetIntFromJSONObject(render_object, "x_limit_margin"); 
	if (m_limit_margin < 0) { return false;  }
	m_speed = App->getConfig().GetIntFromJSONObject(render_object, "speed");
	if (m_speed == 0) { return false; }
	
	return true;
}