#pragma once
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include "../Observer/Observable.h"

class FramesPerSecond : public Observable
{
private:
	int _framesPerSecond;
	int _frameCount;
	float _deltaCount;

public:
	FramesPerSecond();
	FramesPerSecond(const FramesPerSecond& other);
	~FramesPerSecond();

	void Frame(float delta);
	int GetFramesPerSeond() const;
	void AddObserver(IObserver* observer) override;
};