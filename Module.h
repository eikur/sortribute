#ifndef __MODULE_H__
#define __MODULE_H__

#include <string>
#include "3rdparty/parson/parson.h"
#include "3rdparty/SDL/include/SDL.h"

class Application;
class Entity;
struct Collider;

class Module
{
public:

	Module(bool active = true) : active(active)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return active;
	}

	 bool Enable()
	{
		if(active == false)
			return active = Start();

		return true;
	}

	 bool Disable()
	 {
		 if(active == true)
			 return active = !CleanUp();

		 return true;
	 }

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

/*	virtual bool LoadConfigFromFile(const char* )
	{
		return true;
	}
	*/
	// Callbacks ---
	virtual void HandleCollision(Collider*, Collider*)
	{}

protected:
	bool active = true;
};

#endif // __MODULE_H__