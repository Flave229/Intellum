#include "GameTimer.h"


GameTimer::GameTimer()
	: _mSecondsPerCount(0.0), _mDeltaTime(-1.0), _mBaseTime(0), _mPauseTime(0), _mPrevTime(0), _mCurrTime(0), _mStopped(false)
{
	// Finds out the frequency of the performance counter.
	__int64 countsPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
	_mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
}


GameTimer::~GameTimer()
{
}

void GameTimer::Tick()
{
	if (_mStopped) {
		_mDeltaTime = 0.0f;
		return;
	}

	// The the current frames time
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
	_mCurrTime = currTime;

	// Time difference between previous frame and current frame
	_mDeltaTime = (_mCurrTime - _mPrevTime) * _mSecondsPerCount;

	// Overwrite previous frame with current for next frame
	_mPrevTime = _mCurrTime;

	// Protect against non negative delta time
	if (_mDeltaTime < 0.0f) {
		_mDeltaTime = 0.0f;
	}
}

float GameTimer::DeltaTime()const
{
	return static_cast<float>(_mDeltaTime);
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

	_mBaseTime = currTime;
	_mPrevTime = currTime;
	_mStopTime = 0;
	_mStopped = false;
}

void GameTimer::Stop()
{
	if (!_mStopped) {
		__int64 currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

		_mStopTime = currTime;
		_mStopped = true;
	}
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

	if (_mStopped) {
		_mPauseTime += (startTime - _mStopTime);

		_mPrevTime = startTime;

		_mStopTime = 0;
		_mStopped = false;
	}
}