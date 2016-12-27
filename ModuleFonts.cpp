#include "Globals.h"
#include "ModuleFonts.h"

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
		// do stuff
	}
	return res;
}

bool ModuleFonts::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;

	m_font_ids = new int(json_object_dotget_number(json_object(root_value), "fonts.total"));
	if (m_font_ids == nullptr)
		return false;

	//for (int i = 0; i < m_fonts_ids)

	json_value_free(root_value);


	return true;
}

void ModuleFonts::Print(int x, int y, int font_id, const char* text) const
{	
	
}

