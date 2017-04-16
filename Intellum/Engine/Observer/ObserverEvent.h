#pragma once
#include "d3d11.h"
#include "EventType.h"

class ObserverEvent
{
private:
	void* ObservableData;

public:
	EventType EventType;

	template<typename T>
	void SetObservableData(T* data)
	{
		ObservableData = data;
	}

	template<typename T>
	T* GetObservableData()
	{
		return static_cast<T*>(ObservableData);
	}
};