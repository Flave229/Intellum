#include "FramesPerSecond.h"

FramesPerSecond::FramesPerSecond(): _framesPerSecond(0), _frameCount(0), _deltaCount(0)
{
}

FramesPerSecond::FramesPerSecond(const FramesPerSecond& other) : _framesPerSecond(other._framesPerSecond), _frameCount(other._frameCount), _deltaCount(other._deltaCount)
{
}

FramesPerSecond::~FramesPerSecond()
{
}

void FramesPerSecond::Frame(float delta)
{
	_frameCount++;
	_deltaCount = _deltaCount + delta;
	
	if (_deltaCount >= 1000)
	{
		_framesPerSecond = _frameCount;

		_deltaCount = 0;
		_frameCount = 0;
	}
}

int FramesPerSecond::GetFramesPerSeond() const
{
	return _framesPerSecond;
}
