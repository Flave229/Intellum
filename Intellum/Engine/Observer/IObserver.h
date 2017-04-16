#pragma once
#include "ObserverEvent.h"

class IObserver
{
public:
	virtual ~IObserver() { }

	virtual void Notify(ObserverEvent event) = 0;
};