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

	return true;
}

void ModuleFonts::Print(int x, int y, int font_id, const char* text) const
{	

}

