#include "Globals.h"
#include "Animation.h"

#include "ConfigurationLoader.h"

ConfigurationLoader::ConfigurationLoader(const char* config_file_path)
{
	root_value = json_parse_file(config_file_path);
	if (root_value != nullptr)
		root_object = json_object(root_value);
	else
		root_object = nullptr;
}

ConfigurationLoader::~ConfigurationLoader()
{
	json_value_free(root_value);
}

JSON_Object* ConfigurationLoader::GetJSONObject( const char* section_name )
{
	if (strcmp(section_name, "") == 0)
		return root_object;
	const char *found = strchr(section_name, '.');
	if (found == nullptr)
		return json_object_get_object(root_object, section_name);
	else
		return json_object_dotget_object(root_object, section_name); 
}

//--- Load*FromJSONObject : Load and set * variables using the values read in config file

bool ConfigurationLoader::LoadAnimationFromJSONObject(JSON_Object *j_object, const char *animation_name, Animation* animation) const
{
	JSON_Array *j_array = nullptr; 
	JSON_Array *j_array_inner = nullptr; 

	std::string tmp = animation_name;
	tmp.append(".speed");
	animation->speed = (float)json_object_get_number(j_object, tmp.c_str());

	tmp = animation_name;
	tmp.append(".frames");
	j_array = json_object_get_array(j_object, tmp.c_str());
	if (j_array == nullptr)
		return false; 

	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		if (j_array == nullptr)
			return false; 
		animation->frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
	
	json_array_clear(j_array);
	return true; 
}

bool ConfigurationLoader::LoadSDLRectFromJSONObject(JSON_Object* j_object, const char *rect_name, SDL_Rect *rect) const 
{
	JSON_Array *j_array = json_object_get_array(j_object, rect_name); 
	if (j_array == nullptr) { return false; }

	*rect = { (int)json_array_get_number(j_array,0), (int)json_array_get_number(j_array,1), (int)json_array_get_number(j_array,2), (int)json_array_get_number(j_array,3) };
	
	json_array_clear(j_array);
	return true; 
}

bool ConfigurationLoader::LoadiPointFromJSONObject(JSON_Object* j_object, const char *point_name, iPoint *point) const
{
	JSON_Array *j_array = json_object_get_array(j_object, point_name);
	if (j_array == nullptr) { return false; }

	*point = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	json_array_clear(j_array);
	return true;
}



// -- Get* : Only return the values read from the configuration files, no Load and Set
const char* ConfigurationLoader::GetStringFromJSONObject(JSON_Object *j_object, const char *string_name) const
{
	if (json_object_has_value_of_type(j_object, string_name, JSONString))
		return json_object_get_string(j_object, string_name );
	else
		return ""; 
}

int ConfigurationLoader::GetIntFromJSONObject(JSON_Object *j_object, const char* int_name) const
{
	return (int)json_object_get_number(j_object, int_name);
}

float ConfigurationLoader::GetFloatFromJSONObject(JSON_Object *j_object, const char* float_name) const
{
	return (float)json_object_get_number(j_object, float_name);
}

bool ConfigurationLoader::GetBoolFromJSONObject(JSON_Object *j_object, const char* bool_name) const
{
	 return (json_object_get_boolean(j_object, bool_name) != 0) ? true : false;
}