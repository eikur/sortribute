#include "Globals.h"
#include "Timer.h"


Timer::Timer() : Module(true), m_ticks_start(0), m_ticks_pause(0), m_started(false), m_paused(false)
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
	Uint32 delta = 0;

	if (m_started)
	{
		if (m_paused)
			delta = m_ticks_pause;
		else
			delta = SDL_GetTicks() - m_ticks_start;
	}
	return delta;
}

Uint32 Timer::DeltaTime()
{
	return 19;
}