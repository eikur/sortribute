#pragma once

#include <map>
#include <stdlib.h>

struct SDL_Texture;

class TextureHelper
{
public:
	TextureHelper();
	~TextureHelper();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const std::string& path);
	void Unload(SDL_Texture* texture);

private:
	std::map<std::string, SDL_Texture*> _textureMap;
};
