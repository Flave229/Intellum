#include "Cpu.h"

Cpu::Cpu(): _canReadCpu(true), _queryHandle(nullptr), _counterHandle(nullptr), _lastSampleTime(0), _cpuUsage(0)
{
	Initialise();
}

Cpu::Cpu(const Cpu& other) : _canReadCpu(other._canReadCpu), _queryHandle(other._queryHandle), _counterHandle(other._counterHandle), _lastSampleTime(other._lastSampleTime), _cpuUsage(other._cpuUsage)
{
}

Cpu::~Cpu()
{
}

void Cpu::Shutdown()
{
	if (_canReadCpu)
	{
		PdhCloseQuery(_queryHandle);
	}
}

void Cpu::Initialise()
{
	CreatePollCpuUsageQuery();

	PollAllCpuUsage();

	_lastSampleTime = GetTickCount();
	_cpuUsage = 0;
}

void Cpu::CreatePollCpuUsageQuery()
{
	PDH_STATUS status = PdhOpenQuery(nullptr, 0, &_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		_canReadCpu = false;
	}
}

void Cpu::PollAllCpuUsage()
{
	PDH_STATUS status = PdhAddCounter(_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		_canReadCpu = false;
	}
}

void Cpu::Frame()
{
	if (_canReadCpu)
	{
		if ((_lastSampleTime + 1000) < GetTickCount())
		{
			_lastSampleTime = GetTickCount();
			PdhCollectQueryData(_queryHandle);

			PDH_FMT_COUNTERVALUE value;
			PdhGetFormattedCounterValue(_counterHandle, PDH_FMT_LONG, nullptr, &value);

			_cpuUsage = value.longValue;

			for(int i = 0; i < Observers.size(); i++)
			{
				ObserverEvent observerEvent;
				observerEvent.EventType = CPU_USAGE;
				observerEvent.SetObservableData(_cpuUsage);
				Observers.at(i)->Notify(observerEvent);
				observerEvent.Shutdown<long>();
			}
		}
	}
}

int Cpu::GetCpuPercentage()
{
	return _canReadCpu ? static_cast<int>(_cpuUsage) : 0;
}

void Cpu::AddObserver(IObserver* observer)
{
	Observers.push_back(observer);
}
