#pragma once
#include "ObserverEvent.h"

class IObserver
{
public:
	virtual ~IObserver() { }

	virtual void Notify(ObserverEvent<DirectX::XMFLOAT2> event) = 0;
};