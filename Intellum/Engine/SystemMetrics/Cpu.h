#pragma once
#pragma comment(lib, "pdh.lib")

#include <pdh.h>
#include "../Observer/Observable.h"

class Cpu : public Observable
{
private:
	bool _canReadCpu;
	HQUERY _queryHandle;
	HCOUNTER _counterHandle;
	unsigned long _lastSampleTime;
	long _cpuUsage;

	void Initialise();
	void CreatePollCpuUsageQuery();
	void PollAllCpuUsage();

public:
	Cpu();
	Cpu(const Cpu& other);
	~Cpu();
	void Shutdown();

	void Frame();
	int GetCpuPercentage();
	void AddObserver(IObserver* observer) override;
};