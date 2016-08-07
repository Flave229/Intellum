#include "Graphics.h"

Graphics::Graphics(): _direct3D(nullptr), _camera(nullptr), _model(nullptr), _colorShader(nullptr)
{
}

Graphics::Graphics(const Graphics& other) : _direct3D(other._direct3D), _camera(other._camera), _model(other._model), _colorShader(other._colorShader)
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

	_camera = new Camera;
	if (!_camera) return false;

	_camera->SetPosition(0.0f, 0.0f, -5.0f);

	_model = new Model;
	if (!_model) return false;

	result = _model->Initialise(_direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialise the model object", L"Error", MB_OK);
		return false;
	}

	_colorShader = new ColorShader;
	if (!_colorShader) return false;

	result = _colorShader->Initialise(_direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialise the shader class", L"Error", MB_OK);
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

	if(_colorShader)
	{
		_colorShader->Shutdown();
		delete _colorShader;
		_colorShader = nullptr;
	}

	if (_model)
	{
		_model->Shutdown();
		delete _model;
		_model = nullptr;
	}

	if (_camera)
	{
		delete _camera;
		_camera = nullptr;
	}
}

bool Graphics::Frame()
{
	return Render();
}

bool Graphics::Render()
{
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	bool result;

	_direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	_camera->Render();

	_direct3D->GetWorldMatrix(worldMatrix);
	_camera->GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	_model->Render(_direct3D->GetDeviceContext());

	result = _colorShader->Render(_direct3D->GetDeviceContext(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result) return false;

	_direct3D->EndScene();

	return true;
}