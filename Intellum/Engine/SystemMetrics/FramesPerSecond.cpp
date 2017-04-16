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
	_deltaCount += delta;
	
	if (_deltaCount >= 1)
	{
		_framesPerSecond = _frameCount;

		_deltaCount = 0;
		_frameCount = 0;

		for(int i = 0; i < Observers.size(); i++)
		{
			ObserverEvent observerEvent = ObserverEvent();
			observerEvent.EventType = FRAMES_PER_SECOND;
			observerEvent.SetObservableData<int>(&_framesPerSecond);
			Observers.at(i)->Notify(observerEvent);
		}
	}
}

int FramesPerSecond::GetFramesPerSeond() const
{
	return _framesPerSecond;
}

void FramesPerSecond::AddObserver(IObserver* observer)
{
	Observers.push_back(observer);
}