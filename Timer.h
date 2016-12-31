#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL/include/SDL.h"

class Timer {

public:
	void Start(){ 
		ticks_start = SDL_GetTicks(); 
		ticks_last_call = ticks_start;
	}
	void Stop() { 
		ticks_start = 0; 
		ticks_pause = 0; 
		ticks_last_call = 0;
	}
	void Pause(){ 
		ticks_pause = SDL_GetTicks();	
		ticks_last_call = ticks_pause;
	}

	void Resume() {
		ticks_start = ticks_start + SDL_GetTicks() - ticks_pause;
		ticks_pause = 0;
		ticks_last_call = ticks_start;
	}

	void ReStart() {
		Stop();
		Start();
	}

	unsigned int ElapsedTimeMsec()
	{
		ticks_last_call = SDL_GetTicks();
		return ticks_last_call - ticks_start;
	}
	
	unsigned int ElapsedTimeSec()
	{
		return (int)((SDL_GetTicks() - ticks_start) / 1000);
	}

	unsigned int DeltaTime()
	{
		unsigned int tmp = ticks_last_call;
		ticks_last_call = SDL_GetTicks();
		return ticks_last_call - tmp;
	}

private:
	unsigned int ticks_start = 0;
	unsigned int ticks_pause = 0;
	unsigned int ticks_last_call = 0;

};

#endif // __TIMER_H__
