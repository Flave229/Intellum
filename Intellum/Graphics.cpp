#include "Graphics.h"

Graphics::Graphics(): _direct3D(nullptr), _camera(nullptr), _model(nullptr), _shaderController(nullptr),
						_light(nullptr), _bitmap(nullptr)
{
}

Graphics::Graphics(const Graphics& other) : _direct3D(other._direct3D), _camera(other._camera), _model(other._model), _shaderController(other._shaderController),
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
		if (!result) throw Exception("Could not initialise Direct3D.");;

		_camera = new Camera;
		if (!_camera) throw Exception("Failed to create a camera object.");

		_camera->SetPosition(0.0f, 0.0f, -5.0f);

		_shaderController = new ShaderController(_direct3D);
		if (!_shaderController) throw Exception("Failed to create the shader controller.");

		result = _shaderController->Initialise(hwnd, _camera);
		if (!_shaderController) throw Exception("Failed to create the shader controller.");

		_model = new Model(_direct3D, _shaderController->GetShader(SHADER_DEFAULT));
		if (!_model) throw Exception("Failed to create a Model object.");

		result = _model->Initialise("data/images/stone.tga", "data/models/sphere.obj");
		if (!result) throw Exception("Could not initialise the model object.");

		_bitmap = new Bitmap(_direct3D, _shaderController->GetShader(SHADER_FONT));
		if (!_bitmap) throw Exception("Failed to create the bitmap.");

		result = _bitmap->Initialise(screenWidth, screenHeight, 256, 256, "data/images/stone.tga");
		if (!result) throw Exception("Could not initialise the bitmap.");

		_light = new Light;
		if (!_light) throw Exception("Failed to create the light object.");

		_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
		_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		_light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		_light->SetDirection(0.8f, -1.0f, 0.2f);
		_light->SetSpecularPower(32.0f);

		_fontEngine = new FontEngine(_direct3D, _direct3D->GetDevice(), _direct3D->GetDeviceContext(), _shaderController->GetShader(SHADER_FONT));
		if (!_fontEngine) throw Exception("Failed to create the Font Engine.");

		result = _fontEngine->SearchForAvaliableFonts(screenWidth, screenHeight);
		if (!result) throw Exception("Could not initialise Font Engine.");
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

	if(_shaderController)
	{
		_shaderController->Shutdown();
		delete _shaderController;
		_shaderController = nullptr;
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

bool Graphics::Frame(float delta, int mouseX, int mouseY)
{
	try
	{
		return Render(delta, mouseX, mouseY);
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

bool Graphics::Render(float delta, int mouseX, int mouseY)
{
	try
	{
		XMMATRIX viewMatrix;
		bool result;

		_direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		_camera->Render();

		_camera->MapViewMatrixInto(viewMatrix);

		_direct3D->TurnZBufferOff();

		result = _bitmap->Render(_light, 100, 100, 256, 256);
		if (!result) return false;

		result = _fontEngine->Render(_light, 50, 600, "impact", "Victoria Grump", XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f), 30);
		if (!result) return false;

		result = _fontEngine->Render(_light, 10, 10, "impact", "Mouse X: " + to_string(mouseX) + "    " + "Mouse Y: " + to_string(mouseY), XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f), 20);
		if (!result) return false;

		_direct3D->TurnZBufferOn();
		 
		_model->Render(delta, _light);

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