#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "TextureHelper.h"

#include "3rdparty/SDL_image/include/SDL_image.h"
#pragma comment( lib, "3rdparty/SDL_image/libx86/SDL2_image.lib" )

bool TextureHelper::init()
{
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

bool TextureHelper::cleanUp()
{
	for (auto& kvp : _textureMap)
	{
		SDL_DestroyTexture(kvp.second);
	}
	_textureMap.clear();

	IMG_Quit();
	return true;
}

SDL_Texture* const TextureHelper::loadTexture(const std::string& path)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = nullptr; 

	if (path.empty())
	{
		return nullptr;
	}
	const auto it = _textureMap.find(path);
	if (it != _textureMap.cend())
	{
		return it->second;
	}
	
	surface = IMG_Load(path.c_str());
	if(surface == nullptr)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
		return nullptr;
	}

	texture = SDL_CreateTextureFromSurface(App->getRenderer().renderer, surface);
	if(texture == nullptr)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		return nullptr;
	}

	_textureMap[path] = texture;
	SDL_FreeSurface(surface);
	return texture;
}