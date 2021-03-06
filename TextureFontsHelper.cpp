#include "Globals.h"
#include "Application.h"
#include "TextureFontsHelper.h"
#include "TextureHelper.h"
#include "ModuleRender.h"

TextureFontsHelper::TextureFontsHelper()
{
}

TextureFontsHelper::~TextureFontsHelper()
{
	_fontMap.clear();
}

bool TextureFontsHelper::Init()
{
	bool res = true;
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Fonts: Unable to load from config file\n");
		res = false;
	}
	else
	{
		graphics = App->getTextures().Load(asset_file.c_str());
	}
	return res;
}

bool TextureFontsHelper::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	JSON_Object *root_object;
	JSON_Array *j_rect;

	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	//get the path to the asset
	if (json_object_dothas_value_of_type(json_object(root_value), "fonts.file", JSONString))
		asset_file = json_object_dotget_string(json_object(root_value), "fonts.file");
	if (asset_file == "") {
		json_value_free(root_value);
		return false;
	}

	//background load
	Font font;
	
	j_rect = json_object_dotget_array(root_object, "fonts.HudBig.rect");
	font.rect.x = (int)json_array_get_number(j_rect, 0);
	font.rect.y = (int)json_array_get_number(j_rect, 1);
	font.rect.w = (int)json_array_get_number(j_rect, 2);
	font.rect.h = (int)json_array_get_number(j_rect, 3);
	font.lookup_table = json_object_dotget_string(root_object, "fonts.HudBig.lookup_table");
	font.pixels_per_element = (int)json_object_dotget_number(root_object, "fonts.HudBig.ppe");
	font.type = FontType::HudBig;
	_fontMap[FontType::HudBig] = font;

	j_rect = json_object_dotget_array(root_object, "fonts.HudSmall.rect");
	font.rect.x = (int)json_array_get_number(j_rect, 0);
	font.rect.y = (int)json_array_get_number(j_rect, 1);
	font.rect.w = (int)json_array_get_number(j_rect, 2);
	font.rect.h = (int)json_array_get_number(j_rect, 3);
	font.lookup_table = json_object_dotget_string(root_object, "fonts.HudSmall.lookup_table");
	font.pixels_per_element = (int)json_object_dotget_number(root_object, "fonts.HudSmall.ppe");
	font.type = FontType::HudSmall;
	_fontMap[FontType::HudSmall] = font;

	j_rect = json_object_dotget_array(root_object, "fonts.SceneOverlay.rect");
	font.rect.x = (int)json_array_get_number(j_rect, 0);
	font.rect.y = (int)json_array_get_number(j_rect, 1);
	font.rect.w = (int)json_array_get_number(j_rect, 2);
	font.rect.h = (int)json_array_get_number(j_rect, 3);
	font.lookup_table = json_object_dotget_string(root_object, "fonts.SceneOverlay.lookup_table");
	font.pixels_per_element = (int)json_object_dotget_number(root_object, "fonts.SceneOverlay.ppe");
	font.type = FontType::SceneOverlay;
	_fontMap[FontType::SceneOverlay] = font;

	json_value_free(root_value);

	return true;
}

void TextureFontsHelper::Print(int x, int y, FontType fontType, const std::string text) const
{	
	const Font& font = _fontMap.at(fontType);

	// draw the fonts
	int i = 0;
	int i_max = text.length();
	int lut_i;

	SDL_Rect *i_rect = new SDL_Rect();
	i_rect->y = font.rect.y;
	i_rect->w = font.pixels_per_element;
	i_rect->h = font.rect.h;
	
	for (i = 0; i < i_max; ++i)
	{
		lut_i = font.lookup_table.find(text.at(i));
		i_rect->x = font.rect.x + lut_i * font.pixels_per_element;
		App->getRenderer().Blit(graphics, x+i*font.pixels_per_element, y, i_rect, 0.0f);
	}

	delete i_rect;
}

/*---------------------------------------------------------*/

TextureFontsHelper::Font::Font()
{
	type = FontType::Invalid;
	lookup_table = "";
	pixels_per_element = 0;
}

TextureFontsHelper::Font::Font(const Font& f)
{
	rect = SDL_Rect(f.rect);
	type = f.type;
	lookup_table = f.lookup_table;
	pixels_per_element = f.pixels_per_element;
}

TextureFontsHelper::Font::~Font()
{}