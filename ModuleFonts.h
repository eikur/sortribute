#ifndef __MODULEFONTS_H__
#define __MODULEFONTS_H__

#include <vector>
#include <stdlib.h>
#include "Module.h"


struct SDL_Texture;

struct Font {
	int id;
	SDL_Rect *rect;
	std::string lookup_table;
	int pixels_per_element;

	Font();
	Font(const Font& f);
	~Font();
};

class ModuleFonts : public Module
{
public:
	ModuleFonts();
	~ModuleFonts();

	bool Init();
	bool LoadConfigFromFile(const char* file_path);
	bool CleanUp();


	void Print(int x, int y, int font_id, const std::string text) const;
	
private:
	SDL_Texture* graphics = nullptr;
	std::string asset_file = "";
	std::vector<Font*> m_fonts;
};
#endif	// __MODULEFONTS_H__
