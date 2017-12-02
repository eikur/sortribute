#ifndef __MODULEFONTS_H__
#define __MODULEFONTS_H__

#include <map>
#include <stdlib.h>
#include "Module.h"


struct SDL_Texture;

class ModuleFonts : public Module
{
public:
	enum class FontType {
		HudBig, 
		HudSmall,
		SceneOverlay,
		Invalid
	};

	struct Font {
		FontType type;
		SDL_Rect rect;
		std::string lookup_table;
		int pixels_per_element;

		Font();
		Font(const Font& f);
		~Font();
	};

public:
	ModuleFonts();
	~ModuleFonts();

	bool Init();
	bool LoadConfigFromFile(const char* file_path);
	bool CleanUp();

	void Print(int x, int y, FontType fontType, const std::string text) const;
	std::string GetPrintableValue(int value, int desired_length) const;
	
private:
	SDL_Texture* graphics = nullptr;
	std::string asset_file = "";
	std::map<FontType, Font> fonts;
};
#endif	// __MODULEFONTS_H__
