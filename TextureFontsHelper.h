#pragma once

#include <map>
#include <stdlib.h>

struct SDL_Texture;

class TextureFontsHelper
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
	TextureFontsHelper();
	~TextureFontsHelper();

	bool Init();
	bool LoadConfigFromFile(const char* file_path);

	void Print(int x, int y, FontType fontType, const std::string text) const;
	
private:
	SDL_Texture* graphics = nullptr;
	std::string asset_file = "";
	std::map<FontType, Font> _fontMap;
};
