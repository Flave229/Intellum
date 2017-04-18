#pragma once
#include "d3d11.h"
#include "EventType.h"

class ObserverEvent
{
private:
	void* ObservableData;

public:
	EventType EventType;

	ObserverEvent() : ObservableData(nullptr), EventType() { }

	~ObserverEvent() { }

	template<typename T>
	void Shutdown()
	{
		if (ObservableData)
		{
			delete ObservableData;
		}
	}

	template<typename T>
	void SetObservableData(T data)
	{
		ObservableData = new T(data);
	}

	template<typename T>
	T GetObservableData()
	{
		return *static_cast<T*>(ObservableData);
	}
};