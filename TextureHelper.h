#pragma once

#include <list>
#include <stdlib.h>

struct SDL_Texture;
/*
struct Font {
	int id;
	SDL_Rect *rect;
	std::string lookup_table;
	int pixels_per_element;

	Font();
	Font(const Font& f);
	~Font();
};
*/
class TextureHelper
{
public:
	TextureHelper();
	~TextureHelper();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	void Unload(SDL_Texture* texture);

private:
	std::list<SDL_Texture*> textures;
};
