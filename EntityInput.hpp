#pragma once

#include "Globals.h"

class EntityInput
{
public:
	struct Input
	{
		MEMBER_CONST_GET_SET(int, _verticalInput, getVerticalInput, setVerticalInput)
		MEMBER_CONST_GET_SET(int, _horizontalInput, getHorizontalInput, setHorizontalInput)
		MEMBER_CONST_GET_SET(bool, _helpInput, getHelpInput, setHelpInput)
		MEMBER_CONST_GET_SET(bool, _attackInput, getAttackInput, setAttackInput)
		MEMBER_CONST_GET_SET(bool, _jumpInput, getJumpInput, setJumpInput)

		void reset()
		{
			_verticalInput = 0;
			_horizontalInput = 0;
			_helpInput = false;
			_attackInput = false;
			_jumpInput = false;
		}

		void copyFrom(const Input& other)
		{
			_verticalInput = other._verticalInput;
			_horizontalInput = other._horizontalInput;
			_helpInput = other._helpInput;
			_attackInput = other._attackInput;
			_jumpInput = other._jumpInput;
		}
	};

	MEMBER_CONST_GET(Input, _currentInput, getInput)
	MEMBER_CONST_GET(Input, _previousInput, getPreviousInput)

  public:
	virtual void updateInput(){}
	void reset() 
	{
		_currentInput.reset();
		_previousInput.reset();
	}

};