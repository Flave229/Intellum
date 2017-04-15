#pragma once
#include "EventType.h"

template<typename T>
class ObserverEvent
{
private:
	T* ObservableData;
public:
	EventType EventType;

	void SetObservableData(T* data)
	{
		static_cast<T*>(ObservableData) = data;
	}

	T* GetObservableData()
	{
		return static_cast<T*>(ObservableData);
	}
};