#ifndef __CONFIGURATION_LOADER_H__
#define __CONFIGURATION_LOADER_H__

#include <string.h>

#include "3rdparty/parson/parson.h"
#include "Point.h"

class Animation;
struct SDL_Rect;
struct Collider;

class ConfigurationLoader {

public: 
	ConfigurationLoader(const char* config_file_path);
	~ConfigurationLoader();

	JSON_Object* GetJSONObject( const char* section_name = "");

	// Load all JSON OBject data inside appropriate structure
	bool LoadAnimationFromJSONObject(JSON_Object *j_object, const char *animation_name, Animation* animation) const;
	bool LoadSDLRectFromJSONObject(JSON_Object* j_object, const char *rect_name, SDL_Rect *rect) const; 
	bool LoadiPointFromJSONObject(JSON_Object* j_object, const char *point_name, iPoint *point) const;

	// Return values read in JSON Object
	const char* GetStringFromJSONObject(JSON_Object *j_object, const char *string_name) const; 
	int GetIntFromJSONObject(JSON_Object *j_object, const char* int_name) const;
	float GetFloatFromJSONObject(JSON_Object *j_object, const char* float_name) const; 
	bool GetBoolFromJSONObject(JSON_Object *j_object, const char* bool_name) const;

private:
	JSON_Value *root_value = nullptr;
	JSON_Object *root_object = nullptr;
};
#endif
