#include "Globals.h"
#include "Animation.h"

#include "ConfigurationLoader.h"
#include <fstream>
#include <iostream>
#include <string>

namespace
{
	bool isNumber(const char& c)
	{
		return ('0' <= c && c <= '9');
	}
}

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

	std::string line;
	std::ifstream configFile(path);
	
	NodeBase* currentNode = nullptr;
	ParsePhase phase = ParsePhase::Value;
	std::string name;

	if (configFile.is_open())
	{
		while (!configFile.eof())
		{
			std::getline(configFile, line);
			for (size_t i = 0; i < line.size(); ++i)
			{
				char c = line.at(i);

				if (c == '{'  && phase == ParsePhase::Value)
				{
					if (currentNode == nullptr)	// first time only
					{
						rootNode = new NodeBase();
						currentNode = rootNode;
					}
					else
					{
						NodeBase* tmp = new NodeBase();
						tmp->setName(name);
						name.clear();
						currentNode->addChild(std::move(tmp));
						currentNode = tmp;
					}
					phase = ParsePhase::Name;
				}
				else if (c == '}')
				{
					if (currentNode == rootNode)
					{
						break;
					}
					currentNode = currentNode->getParent();
					phase = ParsePhase::Name;
				}
				else if (c == ':')
				{
					// enter parse value mode
					phase = ParsePhase::Value;
				}
				else if (c == '"')
				 {
					 size_t doubleCommaPos = line.find_first_of('"', i + 1);
					 if (phase == ParsePhase::Name)
					 {
						 name = line.substr(i+1, doubleCommaPos-(i+1));
					 }
					 else
					 {
						 // we're dealing with a string node!
						 Node<std::string>* tmp = new Node<std::string>();
						 tmp->setName(name);
						 name.clear();
						 tmp->setValue(line.substr(i+1, doubleCommaPos-(i+1)));
						 currentNode->addChild(std::move(tmp));
						 phase = ParsePhase::Name;
					 }
					 i = doubleCommaPos;
				 }
				else if (isNumber(c))
				{
					// dealing with numbers
					size_t numberEndPos = line.find_last_of("0123456789.");
					Node<double>* tmp = new Node<double>();
					tmp->setName(name);
					name.clear();
					tmp->setValue(std::atof(line.substr(i,numberEndPos-i+1).c_str()));
					currentNode->addChild(std::move(tmp));
					i = numberEndPos + 1;
					phase = ParsePhase::Name;
				}
				else if (c == '[')
				{
					size_t arrayEndPos = line.find_last_of("]");
					//#todo - read array
					Node<std::string>* tmp = new Node<std::string>();
					tmp->setName(name);
					name.clear();
					tmp->setValue(line.substr(i, arrayEndPos - i));
					currentNode->addChild(std::move(tmp));
					i = arrayEndPos + 1;
					phase = ParsePhase::Name;
				}
			}
		}
	}
	configFile.close();
}

ConfigurationLoader::~ConfigurationLoader()
{
	json_value_free(root_value);
	rootNode->cleanUp();
	delete rootNode;
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

bool ConfigurationLoader::LoadAnimationFromJSONObject(JSON_Object *j_object, const std::string& name, Animation* animation) const
{
	JSON_Array *j_array = nullptr; 
	JSON_Array *j_array_inner = nullptr; 

	std::string tmp = name;
	tmp.append(".speed");
	animation->speed = (float)json_object_get_number(j_object, tmp.c_str());

	tmp = name;
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