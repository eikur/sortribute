#include "Timer.h"


Timer::Timer() : _state(State::Stopped)
{}

Timer::~Timer() 
{}

void Timer::TimerStart() 
{
	_state = State::Running;
	_ticksLastStateChange = SDL_GetTicks();
}

void Timer::TimerStop()
{
	_state = State::Stopped;
	_ticksLastStateChange = 0;
}

void Timer::TimerPause() 
{
	if( _state == State::Running)
	{
		_state = State::Paused;
		_ticksLastStateChange = SDL_GetTicks() - _ticksLastStateChange;
	}
}

void Timer::TimerResume() 
{
	if ( _state == State::Paused)
	{
		_state = State::Running;
		_ticksLastStateChange = SDL_GetTicks() - _ticksLastStateChange;
	}
}

Uint32 Timer::getElapsedTime() const 
{
	switch (_state)
	{
	case Timer::State::Paused:
		return _ticksLastStateChange;
	case Timer::State::Running:
		return SDL_GetTicks() - _ticksLastStateChange;
		break;
	case Timer::State::Stopped:
		return 0;
	}
}

void Timer::UpdateDeltaTime()
{
	_ticksDeltaTime = getElapsedTime() - _ticksLastUpdate;
	_ticksLastUpdate = getElapsedTime();
}

float Timer::getDeltaTime() const
{
	return static_cast<float>(_ticksDeltaTime) / 1000.0f;
}

bool Timer::isPaused() const
{
	return _state == State::Paused;
}

bool Timer::isRunning() const
{
	return _state == State::Running;
}