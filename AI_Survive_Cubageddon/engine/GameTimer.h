#pragma once
#include <windows.h>

// Class pulled directly from Frank Luna book. This is not my code.
class GameTimer
{
private:
	double _mSecondsPerCount;
	double _mDeltaTime;

	__int64 _mBaseTime;
	__int64 _mStopTime;
	__int64 _mPauseTime;
	__int64 _mPrevTime;
	__int64 _mCurrTime;

	bool _mStopped;
public:
	GameTimer();
	~GameTimer();

	float GameTime()const;
	float DeltaTime()const;

	void Reset();
	void Start();
	void Stop();
	void Tick();
};

