#include "Graphics.h"
#include "error_handling/Exception.h"

Graphics::Graphics(): _direct3D(nullptr), _camera(nullptr), _model(nullptr), _shader(nullptr), 
						_light(nullptr), _bitmap(nullptr)
{
}

Graphics::Graphics(const Graphics& other) : _direct3D(other._direct3D), _camera(other._camera), _model(other._model), _shader(other._shader), 
												_light(other._light), _bitmap(other._bitmap)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialise(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	try
	{
		_direct3D = new DirectX3D;
		if (!_direct3D) throw Exception("Failed to create a DirectX3D object.");

		result = _direct3D->Initialise(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
		if (!result) throw Exception("Could not initialise Direct3D.");

		_camera = new Camera;
		if (!_camera) throw Exception("Failed to create a camera object.");

		_camera->SetPosition(0.0f, 0.0f, -5.0f);

		_model = new Model;
		if (!_model) throw Exception("Failed to create a Model object.");

		result = _model->Initialise(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), "data/images/stone.tga", "data/models/sphere.obj");
		if (!result) throw Exception("Could not initialise the model object.");

		_shader = new ShaderController;
		if (!_shader) throw Exception("Failed to create the shader controller.");

		result = _shader->Initialise(_direct3D->GetDevice(), hwnd);
		if (!result) throw Exception("Could not initialise the shader controller.");

		_bitmap = new Bitmap;
		if (!_bitmap) throw Exception("Failed to create the bitmap.");

		result = _bitmap->Initialise(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), screenWidth, screenHeight, 256, 256, "data/images/stone.tga");
		if (!result) throw Exception("Could not initialise the bitmap.");

		_light = new Light;
		if (!_light) throw Exception("Failed to create the light object.");

		_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
		_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		_light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		_light->SetDirection(0.8f, -1.0f, 0.2f);
		_light->SetSpecularPower(32.0f);
	}
	catch(Exception& exception)
	{
		throw Exception("Failed to initialise the graphics class.", exception);
	}
	catch(...)
	{
		throw Exception("Failed to initialise the graphics class.");
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

	if(_shader)
	{
		_shader->Shutdown();
		delete _shader;
		_shader = nullptr;
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

	if (_light)
	{
		delete _light;
		_light = nullptr;
	}

	if (_bitmap)
	{
		_bitmap->Shutdown();
		delete _bitmap;
		_bitmap = nullptr;
	}
}

bool Graphics::Frame(float delta)
{
	try
	{
		static float rotation = 0.0f;

		rotation += static_cast<float>(XM_PI) * 0.5f * delta;

		if (rotation > 360.0f)
		{
			rotation -= 360.0f;
		}

		return Render(rotation);
	}
	catch (Exception& exception)
	{
		throw Exception("Frame failed to render.", exception);
	}
	catch(...)
	{
		throw Exception("An unexpected error occured while rendering the frame");
	}
}

bool Graphics::Render(float rotation)
{
	try
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
		XMMATRIX orthoMatrix;
		bool result;

		_direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		_camera->Render();

		_direct3D->GetWorldMatrix(worldMatrix);
		_camera->GetViewMatrix(viewMatrix);
		_direct3D->GetProjectionMatrix(projectionMatrix);
		_direct3D->GetOrthoMatrix(orthoMatrix);

		_direct3D->TurnZBufferOff();

		result = _bitmap->Render(_direct3D->GetDeviceContext(), 100, 100);
		if (!result) return false;

		result = _shader->Render(_direct3D->GetDeviceContext(), _bitmap->GetIndexCount(), worldMatrix, viewMatrix,
			orthoMatrix, _bitmap->GetTexture(), _light->GetDirection(), _camera->GetPosition(),
			_light->GetAmbientColor(), _light->GetDiffuseColor(), _light->GetSpecularColor(), _light->GetSpecularPower());
		if (!result) return false;

		_direct3D->TurnZBufferOn();

		worldMatrix *= XMMatrixRotationY(rotation);

		_model->Render(_direct3D->GetDeviceContext());

		result = _shader->Render(_direct3D->GetDeviceContext(), _model->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, _model->GetTexture(), _light->GetDirection(), _camera->GetPosition(),
			_light->GetAmbientColor(), _light->GetDiffuseColor(), _light->GetSpecularColor(), _light->GetSpecularPower());
		if (!result) return false;

		_direct3D->EndScene();

		return true;
	}
	catch(Exception& exception)
	{
		throw exception;
	}
	catch(...)
	{
		throw new Exception("An unexpected error occured while rendering the frame");
	}
}