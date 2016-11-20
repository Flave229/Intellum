#pragma once
#pragma comment(lib, "pdh.lib")

#include <pdh.h>

class Cpu
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
};