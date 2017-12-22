#pragma once

#include "3rdparty\SDL\include\SDL_timer.h"

class Timer{

	enum class State
	{
		Paused,
		Running,
		Stopped
	};
public:
	Timer();
	~Timer();

	void TimerStart();
	void TimerStop();
	void TimerPause();
	void TimerResume();

	void UpdateDeltaTime();
	float getDeltaTime() const;

	bool isPaused() const;
	bool isRunning() const;

private:
	Uint32 getElapsedTime() const;

private:
	Uint32 _ticksLastStateChange = 0;
	Uint32 _ticksLastUpdate = 0;
	Uint32 _ticksDeltaTime = 0;

	State _state = State::Stopped;
};
