#pragma once

#include "Globals.h"
#include "EntityInput.hpp"

class EntityStatus
{
	class EntityState {};
	EntityState _currentState;
	EntityState _previousState;

public:
	virtual void statusUpdateAfterInput(const EntityInput& input) = 0;
	virtual void setState(EntityState& state) = 0;

};