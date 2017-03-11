#include "Globals.h"
#include "Animation.h"
#include "Point.h"

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
	if (root_value != nullptr)
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