#pragma once
#include <d3d11.h>

#include "../../ErrorHandling/Exception.h"

class HardwareDescription
{
private:
	int _videoCardMemory;
	char _videoCardDescription[128];

	unsigned int _refreshRateNumerator;
	unsigned int _refreshRateDenominator;

	void Initialise(int screenWidth, int screenHeight);
public:
	HardwareDescription(int screenWidth, int screenHeight);
	HardwareDescription(const HardwareDescription& other);
	~HardwareDescription();


	unsigned int GetRefreshRateNumerator() const;
	unsigned int GetRefreshRateDenominator() const;

	void MapVideoCardMemoryInto(int&) const;
	void GetVideoCardDescription(char*);
};

