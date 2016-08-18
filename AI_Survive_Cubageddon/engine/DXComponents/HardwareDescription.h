#pragma once

#include <d3d11.h>

class HardwareDescription
{
// Member Level Variables
private:
	int _videoCardMemory;
	char _videoCardDescription[128];

	unsigned int _refreshRateNumerator;
	unsigned int _refreshRateDenominator;
public:
	HardwareDescription();
	HardwareDescription(const HardwareDescription&);
	~HardwareDescription();

	bool Initialise(int, int);

	unsigned int GetRefreshRateNumerator() const;
	unsigned int GetRefreshRateDenominator() const;

	void GetVideoCardMemory(int&) const;
	void GetVideoCardDescription(char*);
};

