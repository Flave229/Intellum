#include "HardwareDescription.h"

HardwareDescription::HardwareDescription(): _videoCardMemory(0) {
}

HardwareDescription::HardwareDescription(const HardwareDescription& other): _videoCardMemory(other._videoCardMemory)
{
}

HardwareDescription::~HardwareDescription()
{
}

bool HardwareDescription::Initialise(int screenWidth, int screenHeight)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes;
	unsigned long long stringLength;
	DXGI_ADAPTER_DESC adapterDesc;

	// Creates the graphics interface factory for DirectX
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
	if (FAILED(result)) return false;

	// Creates an adapter for the users GPU (Graphical Processing Unit) from the factory
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	// Enumerate primary adapter output
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result)) return false;

	// Creates a list of all possible combinations of monitor and GPU combinations
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// Fill out above list
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) return false;

	// Search all display modes for one that matches the set screen width and height
	for (int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == static_cast<int>(screenWidth))
		{
			if (displayModeList[i].Height == static_cast<int>(screenHeight))
			{
				_refreshRateNumerator = displayModeList[i].RefreshRate.Numerator;
				_refreshRateDenominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the description for the GPU
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	// Get the video card memory in megabytes
	_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	int error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0) return false;

	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	return true;
}

unsigned HardwareDescription::GetRefreshRateNumerator() const
{
	return _refreshRateNumerator;
}

unsigned HardwareDescription::GetRefreshRateDenominator() const
{
	return _refreshRateDenominator;
}

void HardwareDescription::GetVideoCardMemory(int& memory) const
{
	memory = _videoCardMemory;
}

void HardwareDescription::GetVideoCardDescription(char* cardname)
{
	cardname = _videoCardDescription;
}
