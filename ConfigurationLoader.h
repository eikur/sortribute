#pragma once

#include <string.h>

#include "3rdparty/parson/parson.h"
#include "Point.h"

class Animation;
struct SDL_Rect;
struct Collider;

enum class ParsePhase
{
	Name,
	Value
};

class NodeBase
{
	std::string _name;
	NodeBase* _parent = nullptr;
protected:
	std::vector<NodeBase*> _children;
public:
	NodeBase* getParent() const
	{
		return _parent;
	}

	void addChild(NodeBase* node)
	{
		node->_parent = this;
		_children.push_back(node);
	}
	void setName(const std::string& name)
	{
		_name = name;
	}
	virtual void cleanUp()
	{
		for (auto child : _children)
		{
			child->cleanUp();
			delete child;
		}
	}
};

template <class T>
class Node : public NodeBase
{
	T* _value = nullptr;
	bool _isArray = false;
public:
	bool isArray() const
	{
		return _isArray;
	}
	void setValue(const T& value)
	{
		_value = new T(value);
	}
	const T& getValue() const
	{
		return _value;
	}
	void cleanUp() override
	{
		for (auto child : _children)
		{
			child->cleanUp();
			delete child;
		}
		delete _value;
	}
};

class ConfigurationLoader 
{
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

	NodeBase* rootNode = nullptr;
};
