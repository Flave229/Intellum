#include "Graphics.h"

Graphics::Graphics(): _direct3D(nullptr)
{
}

Graphics::Graphics(const Graphics&)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialise(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	_direct3D = new DirectX3D;
	if (!_direct3D) return false;

	result = _direct3D->Initialise(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialise Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	if(_direct3D)
	{
		_direct3D->Shutdown();
		delete _direct3D;
		_direct3D = nullptr;
	}
}

bool Graphics::Frame()
{
	return Render();
}

bool Graphics::Render()
{
	_direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	_direct3D->EndScene();

	return true;
}