#pragma once
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>

class FramesPerSecond
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
};