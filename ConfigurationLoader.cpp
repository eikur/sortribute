#include "Globals.h"
#include "Animation.h"

#include "ConfigurationLoader.h"

ConfigurationLoader::ConfigurationLoader(const std::string& path)
{
	root_value = json_parse_file(path.c_str());
	if (root_value != nullptr)
	{
		root_object = json_object(root_value);
	}
	else
	{
		root_object = nullptr;
	}
}

ConfigurationLoader::~ConfigurationLoader()
{
	json_value_free(root_value);
}

JSON_Object* ConfigurationLoader::GetJSONObject(const std::string& sectionName)
{
	if (sectionName.empty())
	{
		return root_object;
	}
	if (sectionName.find(".", 0))
	{
		return json_object_dotget_object(root_object, sectionName.c_str());
	}
	else
	{
		return json_object_get_object(root_object, sectionName.c_str());
	}
}

bool ConfigurationLoader::LoadAnimationFromJSONObject(JSON_Object *j_object, const std::string& dotget_path, Animation* animation) const
{
	JSON_Array *j_array, *j_array_inner, *j_array_normTimes;
	std::string tmp = dotget_path;

	tmp.append(".duration");
	animation->duration = (float)json_object_dotget_number(j_object, tmp.c_str());

	tmp = dotget_path;
	tmp.append(".frames");
	j_array = json_object_dotget_array(j_object, tmp.c_str());

	tmp = dotget_path;
	tmp.append(".norm_times");
	j_array_normTimes = json_object_dotget_array(j_object, tmp.c_str());

	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		TimedFrame frame(json_array_get_number(j_array_normTimes, i),
		{ (int)json_array_get_number(j_array_inner, 0),
			(int)json_array_get_number(j_array_inner, 1),
			(int)json_array_get_number(j_array_inner, 2),
			(int)json_array_get_number(j_array_inner, 3) }
		);
		animation->frames.push_back(frame);
		json_array_clear(j_array_inner);
	}
	return true; 
}

bool ConfigurationLoader::LoadSDLRectFromJSONObject(JSON_Object* j_object, const std::string& name, SDL_Rect *rect) const 
{
	JSON_Array *j_array = json_object_get_array(j_object, name.c_str()); 
	if (j_array == nullptr) { return false; }

	*rect = { (int)json_array_get_number(j_array,0), (int)json_array_get_number(j_array,1), (int)json_array_get_number(j_array,2), (int)json_array_get_number(j_array,3) };
	
	json_array_clear(j_array);
	return true; 
}

bool ConfigurationLoader::LoadiPointFromJSONObject(JSON_Object* j_object, const std::string& name, iPoint *point) const
{
	JSON_Array *j_array = json_object_get_array(j_object, name.c_str());
	if (j_array == nullptr) { return false; }

	*point = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	json_array_clear(j_array);
	return true;
}

std::string ConfigurationLoader::GetStringFromJSONObject(JSON_Object *j_object, const std::string& name) const
{
	if (json_object_has_value_of_type(j_object, name.c_str(), JSONString))
	{
		return json_object_get_string(j_object, name.c_str());
	}
	else
	{
		return "";
	}
}

int ConfigurationLoader::GetIntFromJSONObject(JSON_Object *j_object, const std::string& name) const
{
	return static_cast<int>(json_object_get_number(j_object, name.c_str()));
}

float ConfigurationLoader::GetFloatFromJSONObject(JSON_Object *j_object, const std::string& name) const
{
	return static_cast<float>(json_object_get_number(j_object, name.c_str()));
}

bool ConfigurationLoader::GetBoolFromJSONObject(JSON_Object *j_object, const std::string& name) const
{
	 return (json_object_get_boolean(j_object, name.c_str()) != 0) ? true : false;
}