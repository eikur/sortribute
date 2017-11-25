#pragma once

#include "3rdparty\SDL\include\SDL_timer.h"

class Timer{

public:
	Timer();
	~Timer();

	void TimerStart();
	void TimerStop();
	void TimerPause();
	void TimerResume();

	void UpdateDeltaTime();
	float getDeltaTime() const;

private:
	Uint32 getElapsedTime() const;

private:
	Uint32 m_ticks_start = 0;
	Uint32 m_ticks_pause = 0;
	Uint32 m_ticks_last_update = 0;
	Uint32 m_delta_time = 0;

	bool m_started = false;
	bool m_paused = false;
	

};
