#ifndef __CONFIGURATION_LOADER_H__
#define __CONFIGURATION_LOADER_H__

#include <string.h>

#include "3rdparty/parson/parson.h"

class Animation;
struct Collider;
struct SDL_Rect;

class ConfigurationLoader {

public: 
	ConfigurationLoader(const char* config_file_path);
	~ConfigurationLoader();

	JSON_Object* GetJSONObject( const char* section_name = "");

	bool LoadAnimationFromJSONObject(JSON_Object *j_object, const char *animation_name, Animation* animation) const;

private:
	JSON_Value *root_value = nullptr;
	JSON_Object *root_object = nullptr;
};
#endif
