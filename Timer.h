#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL/include/SDL.h"

class Timer {

public:
	void Start(){ 
		ticks_start = SDL_GetTicks(); 
	}
	void Stop() { 
		ticks_start = 0; 
		ticks_pause = 0; 
	}
	void Pause(){ 
		ticks_pause = SDL_GetTicks();	
	}

	void Resume() {
		ticks_start = ticks_start + SDL_GetTicks() - ticks_pause;
		ticks_pause = 0;
	}

	unsigned int ElapsedTimeMsec()
	{
		return SDL_GetTicks() - ticks_start;
	}


private:
	unsigned int ticks_start = 0;
	unsigned int ticks_pause = 0;

};

#endif // __TIMER_H__
