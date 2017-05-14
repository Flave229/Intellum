#include "Graphics.h"

Graphics::Graphics(Input* input, Box screenSize, HWND hwnd, FramesPerSecond* framesPerSecond, Cpu* cpu) : _direct3D(nullptr), _fontEngine(nullptr), _shaderController(nullptr), _objectHandler(nullptr), _camera(nullptr), _light(nullptr)
{
	Initialise(input, framesPerSecond, cpu, screenSize, hwnd);
}

Graphics::~Graphics()
{
}

void Graphics::Initialise(Input* input, FramesPerSecond* framesPerSecond, Cpu* cpu, Box screenSize, HWND hwnd)
{
	try
	{
		_direct3D = new DirectX3D(input, screenSize, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
		if (!_direct3D) throw Exception("Failed to create a DirectX3D object.");
		
		_camera = new Camera(new Frustrum(_direct3D), new Transform(_direct3D), input);
		if (!_camera) throw Exception("Failed to create a camera object.");

		_camera->GetTransform()->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));

		_shaderController = new ShaderController(_direct3D);
		if (!_shaderController) throw Exception("Failed to create the shader controller.");

		_light = new Light;
		if (!_light) throw Exception("Failed to create the light object.");

		bool result = _shaderController->Initialise(hwnd, _camera, _light);
		if (!_shaderController) throw Exception("Failed to create the shader controller.");

		Transform* lightTransformation = new Transform(_direct3D);
		lightTransformation->SetPosition(XMFLOAT3(1000.0f, 600.0f, 40.0f));
		lightTransformation->SetAngularVelocity(XMFLOAT3(0.0f, 0.0f, 10.0f));
		_light->Transform = lightTransformation;
		_light->AmbientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
		_light->DiffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		_light->SpecularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//_light->Direction = XMFLOAT3(0.8f, -1.0f, 0.2f);
		_light->SpecularPower = 32.0f;

		_fontEngine = new FontEngine(_direct3D, _direct3D->GetDevice(), _direct3D->GetDeviceContext(), _shaderController->GetShader(SHADER_FONT));
		if (!_fontEngine) throw Exception("Failed to create the Font Engine.");
		
		_objectHandler = new ObjectHandler(_direct3D, _shaderController, _fontEngine, hwnd, _camera, input, framesPerSecond, cpu, screenSize);
		if (!_objectHandler) throw Exception("Failed to create the object handler.");

		result = _fontEngine->SearchForAvaliableFonts(screenSize);
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

	if (_objectHandler)
	{
		_objectHandler->Shutdown();
		delete _objectHandler;
		_objectHandler = nullptr;
	}

	if (_camera)
	{
		_camera->Shutdown();
		delete _camera;
		_camera = nullptr;
	}

	if (_light)
	{
		delete _light;
		_light = nullptr;
	}
}

void Graphics::Update(float delta) const
{
	try
	{
		_direct3D->Update(delta);
		_camera->Update(delta);
		_objectHandler->Update(delta);
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

void Graphics::Render() const
{
	try
	{
		_direct3D->BeginScene(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		_objectHandler->Render();		 
		_direct3D->EndScene();
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
