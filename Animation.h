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

	const float getNormTime() const
	{
		return _normTime;
	}
};

class Animation
{
public:
	bool loop = true;
	float duration = 1.0f;

	std::vector<TimedFrame> frames;

private:
	float _animationTime = 0.0f;

public:
	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), duration(anim.duration), frames(anim.frames)
	{}

	const SDL_Rect& getCurrentFrame() const
	{
		float normalizedTime = _animationTime / duration;
		normalizedTime = MIN(normalizedTime, 1.0f);

		int retIndex = 0;
		for (int i = 0; i < frames.size(); ++i)
		{
			if (frames.at(i).getNormTime() <= normalizedTime)
			{
				retIndex = i; 
			}
			else
			{
				break;
			}
		}
		return frames.at(retIndex).getRect();
	}

	void SetCurrentFrame(int frame_num)
	{
		if (frame_num >= frames.size())
		{
			return;
		}
		float normTime = frames.at(frame_num).getNormTime();
		_animationTime = duration * normTime;
	}

	void Reset()
	{
		_animationTime = 0.0f;
	}

	void updateAnimationTime(float dt)
	{
		// todo - improve this
		if (dt <= 0.0003f)
		{
			return;
		}
		_animationTime += dt;
		if (_animationTime >= duration)
		{
			_animationTime = (loop) ? _animationTime - duration : duration;
		}

	}

};
