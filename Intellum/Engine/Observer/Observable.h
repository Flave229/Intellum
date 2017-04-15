#pragma once
#include "IObserver.h"
#include <vector>

class Observable
{
public:
	std::vector<IObserver*> Observers;

	virtual void AddObserver(IObserver* observer) = 0;
	virtual ~Observable() {}
};
