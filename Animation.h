#pragma once 

#include <vector>

class TimedFrame
{
	float _normTime = 0.0f;
	SDL_Rect _frame;

public:
	TimedFrame(float normTime, const SDL_Rect& frame) : _normTime(normTime), _frame(frame)
	{}

	const SDL_Rect& getRect() const
	{
		return _frame;
	}
};

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	float duration = 1.0f;

	std::vector<TimedFrame> frames;

private:
	float current_frame = 0.0f;
	float currentSpeed = 0.0f;
	int loops = 0;

public:
	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), frames(anim.frames)
	{}

	const SDL_Rect& GetCurrentFrame()
	{
		float last_frame = (float) frames.size();

		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : MAX(last_frame - 1.0f, 0.0f);
			loops++;
		} 

		return frames[(int)current_frame].getRect();
	}

	void SetCurrentFrame(int frame_num)
	{
		current_frame = (float)frame_num;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
	}
};
