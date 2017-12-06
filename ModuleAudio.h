#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include <vector>
#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	bool PlayMusic(const std::string& path, float fade_time = DEFAULT_MUSIC_FADE_TIME);
	unsigned int LoadFx(const std::string& path);
	bool PlayFx(unsigned int fx, int repeat = 0);

private:
	Mix_Music*	music = nullptr;
	std::vector<Mix_Chunk*>	fx;
};

#endif // __MODULEAUDIO_H__