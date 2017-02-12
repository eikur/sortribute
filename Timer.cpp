#include "Timer.h"


Timer::Timer() : m_ticks_start(0), m_ticks_pause(0), m_started(false), m_paused(false)
{}

Timer::~Timer() {}

void Timer::TimerStart() {
	m_started = true;
	m_paused = false;

	m_ticks_start = SDL_GetTicks();
	m_ticks_pause = 0;
}

void Timer::TimerStop() {
	m_started = false;
	m_paused = false;

	m_ticks_start = 0;
	m_ticks_pause = 0;
}

void Timer::TimerPause() {
	if (m_started && !m_paused)
	{
		m_paused = true;

		m_ticks_pause = SDL_GetTicks() - m_ticks_start;
		m_ticks_start = 0;
	}
}

void Timer::TimerResume() {
	if (m_started && m_paused)
	{
		m_paused = false;
		m_ticks_start = SDL_GetTicks() - m_ticks_pause;
		m_ticks_pause = 0;
	}
}

Uint32 Timer::ElapsedTime()
{
	Uint32 elapsed_time = 0;

	if (m_started)
	{
		if (m_paused)
			elapsed_time = m_ticks_pause;
		else
			elapsed_time = SDL_GetTicks() - m_ticks_start;
	}
	return elapsed_time;
}


void Timer::UpdateDeltaTime()
{
	m_delta_time = ElapsedTime() - m_ticks_last_update;
	m_ticks_last_update = ElapsedTime();

}


Uint32 Timer::DeltaTime()
{
	return m_delta_time;
}