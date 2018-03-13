#pragma once

#include <map>
#include <stdlib.h>

struct SDL_Texture;

class TextureHelper
{
public:
	bool init();
	bool cleanUp();
	SDL_Texture* const loadTexture(const std::string& path);

private:
	std::map<std::string, SDL_Texture*> _textureMap;
};
