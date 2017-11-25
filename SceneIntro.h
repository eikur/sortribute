#pragma once

#include "Scene.h"
#include "SceneManager.h"

struct SDL_Texture;

class SceneIntro : public Scene
{
public:
	SceneIntro(SceneManager& manager);
	~SceneIntro();

	bool Init(); 
	bool Start();
	UpdateStatus Update(float dt = 0.0f) override;
	bool CleanUp();

private:
	bool LoadConfigFromFile();

private:
	SDL_Texture* background = nullptr;
	std::string music_path = "";
	unsigned int fx_start = 0;

	uint blink_msg_msec = 0;
	uint elapsed_msec = 0;
};
