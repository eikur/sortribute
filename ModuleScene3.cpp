#include "Globals.h"
#include "ModuleScene3.h"

ModuleScene3::ModuleScene3(bool active) : Module(active)
{
}

ModuleScene3::~ModuleScene3()
{}


bool ModuleScene3::Init()
{
	LOG("Scene3: Initialisation of Moon Beach Scene\n");

	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		LOG("Scene3: Unable to load config from file\n");
		return false;
	}
	else
	{
		// init some stuff
	}
	return true;

}

bool ModuleScene3::Start()
{
	LOG("Scene3: Starting Moon Beach Scene\n");
	// start some stuff
	return true;
}



update_status ModuleScene3::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleScene3::CleanUp()
{
	LOG("Scene3: Unloading Moon Beach scene\n");
	// unload assets
	return true;
}

bool ModuleScene3::LoadConfigFromFile(const char* file_path)
{
	return true;
}