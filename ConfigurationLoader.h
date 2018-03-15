#pragma once

#include <string.h>

#include "3rdparty/parson/parson.h"
#include "Point.h"

class Animation;
class Collider;
struct SDL_Rect;

class ConfigurationLoader {

public: 
	ConfigurationLoader(const std::string& path);
	~ConfigurationLoader();

	JSON_Object* GetJSONObject(const std::string& sectionName);

	// Load all JSON OBject data inside appropriate structure
	bool LoadAnimationFromJSONObject(JSON_Object *j_object, const std::string& name, Animation* animation) const;
	bool LoadSDLRectFromJSONObject(JSON_Object* j_object, const std::string& name, SDL_Rect *rect) const; 
	bool LoadiPointFromJSONObject(JSON_Object* j_object, const std::string& name, iPoint *point) const;

	// Return values read in JSON Object
	std::string GetStringFromJSONObject(JSON_Object *j_object, const std::string& name) const; 
	int GetIntFromJSONObject(JSON_Object *j_object, const std::string& name) const;
	float GetFloatFromJSONObject(JSON_Object *j_object, const std::string& name) const; 
	bool GetBoolFromJSONObject(JSON_Object *j_object, const std::string& name) const;

private:
	JSON_Value *root_value = nullptr;
	JSON_Object *root_object = nullptr;
};
