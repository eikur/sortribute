#include "Globals.h"
#include "Application.h"
#include "ModuleFonts.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

ModuleFonts::ModuleFonts()
{
}

ModuleFonts::~ModuleFonts()
{
}

bool ModuleFonts::Init()
{
	bool res = true;
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Fonts: Unable to load from config file\n");
		res = false;
	}
	else
	{
		graphics = App->textures->Load(asset_file.c_str());
	}
	return res;
}

bool ModuleFonts::CleanUp()
{
	LOG("Fonts: Unloading fonts\n");

	App->textures->Unload(graphics);

	for (std::vector<Font*>::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it)
		RELEASE(*it);
	m_fonts.clear();
	
	return true;

}

bool ModuleFonts::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	JSON_Array *j_rects;
	JSON_Array *j_rect;
	JSON_Array *j_lookup;
	JSON_Array *j_ppe;

	if (root_value == nullptr)
		return false;

	int total = (int) json_object_dotget_number(json_object(root_value), "fonts.total");
	if (total == 0)
		return false;

	//get the path to the asset
	asset_file = json_object_dotget_string(json_object(root_value), "fonts.file");

	//get each fonts 
	j_rects = json_object_dotget_array(json_object(root_value), "fonts.rects");
	j_lookup = json_object_dotget_array(json_object(root_value), "fonts.lookup_table");
	j_ppe = json_object_dotget_array(json_object(root_value), "fonts.ppe");

	Font *tmp = new Font();

	for (int i = 0; i < total; ++i)
	{
		tmp->id = i;

		j_rect = json_array_get_array(j_rects, i);
		tmp->rect->x = (int)json_array_get_number(j_rect, 0);
		tmp->rect->y = (int)json_array_get_number(j_rect, 1);
		tmp->rect->w = (int)json_array_get_number(j_rect, 2);
		tmp->rect->h = (int)json_array_get_number(j_rect,  3);
		
		tmp->lookup_table = json_array_get_string(j_lookup, i);
		
		tmp->pixels_per_element = (int) json_array_get_number(j_ppe, i);

		m_fonts.push_back(new Font(*tmp));
	}

	delete tmp;
	json_value_free(root_value);


	return true;
}

void ModuleFonts::Print(int x, int y, int font_id, const std::string text) const
{	
	// find selected font
	std::vector<Font*>::const_iterator it;

	for (it = m_fonts.cbegin(); it != m_fonts.cend(); ++it)
		if ((*it)->id == font_id)
			break;

	// draw the fonts
	int i = 0;
	int i_max = text.length() + 1;
	int lut_i;

	SDL_Rect *i_rect;
	i_rect->y = (*it)->rect->y;
	i_rect->w = (*it)->rect->w;
	i_rect->h = (*it)->rect->h;
	
	for (i = 0; i < i_max; ++i)
	{
		lut_i = (*it)->lookup_table.find(text.at(i));
		i_rect->x = lut_i * (*it)->pixels_per_element;
		App->renderer->Blit(graphics, x+i*(*it)->pixels_per_element, y, i_rect, 1.0f);
	}
}

/*---------------------------------------------------------*/

Font::Font()
{
	rect = new SDL_Rect();
	id = 0;
	lookup_table = "";
	pixels_per_element = 0;
}

Font::Font(const Font& f)
{
	rect = new SDL_Rect(*f.rect);
	id = f.id;
	lookup_table = f.lookup_table;
	pixels_per_element = f.pixels_per_element;
}

Font::~Font()
{
	delete rect;
}