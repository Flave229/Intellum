#include "Graphics.h"

Graphics::Graphics(Box screenSize, HWND hwnd, FramesPerSecond* framesPerSecond, Cpu* cpu) : _direct3D(nullptr), _fontEngine(nullptr), _framesPerSecond(framesPerSecond), _cpu(cpu), _shaderController(nullptr), _objectHandler(nullptr), _camera(nullptr), _light(nullptr), _bitmap(nullptr)
{
	Initialise(screenSize, hwnd);
}

Graphics::Graphics(const Graphics& other) : _direct3D(other._direct3D), _fontEngine(nullptr), _framesPerSecond(nullptr), _cpu(other._cpu), _shaderController(other._shaderController), _objectHandler(other._objectHandler), _camera(other._camera), _light(other._light), _bitmap(other._bitmap)
{
}

Graphics::~Graphics()
{
}

void Graphics::Initialise(Box screenSize, HWND hwnd)
{
	try
	{
		_direct3D = new DirectX3D(screenSize, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
		if (!_direct3D) throw Exception("Failed to create a DirectX3D object.");

		_camera = new Camera(new Transform(_direct3D));
		if (!_camera) throw Exception("Failed to create a camera object.");

		_camera->GetTransform()->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));

		_shaderController = new ShaderController(_direct3D);
		if (!_shaderController) throw Exception("Failed to create the shader controller.");

		_light = new Light;
		if (!_light) throw Exception("Failed to create the light object.");

		bool result = _shaderController->Initialise(hwnd, _camera, _light);
		if (!_shaderController) throw Exception("Failed to create the shader controller.");

		_objectHandler = new ObjectHandler(_direct3D, _shaderController);
		if (!_objectHandler) throw Exception("Failed to create the object handler.");
		
		_bitmap = new Bitmap(_direct3D, _shaderController->GetShader(SHADER_FONT), screenSize, Box(256, 256), "data/images/stone.tga");
		if (!_bitmap) throw Exception("Failed to create the bitmap.");

		_light->SetAmbientColor(XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f));
		_light->SetDiffuseColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		_light->SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		_light->SetDirection(XMFLOAT3(0.8f, -1.0f, 0.2f));
		_light->SetSpecularPower(32.0f);

		_fontEngine = new FontEngine(_direct3D, _direct3D->GetDevice(), _direct3D->GetDeviceContext(), _shaderController->GetShader(SHADER_FONT));
		if (!_fontEngine) throw Exception("Failed to create the Font Engine.");

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

void Graphics::Update(float delta) const
{
	try
	{
		_objectHandler->Update(delta);
		_bitmap->Update(XMFLOAT2(100, 100), Box(256, 256));
		_camera->Update(delta);
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

void Graphics::Render(XMFLOAT2 mousePoint) const
{
	try
	{
		_direct3D->BeginScene(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		_direct3D->TurnZBufferOff();

		_bitmap->Render();

		_fontEngine->Update(XMFLOAT2(50, 600), "Impact", "Victoria Grump", XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f), 30);
		_fontEngine->Update(XMFLOAT2(10, 10), "Impact", "Mouse X: " + to_string(static_cast<int>(mousePoint.x)) + "    " + "Mouse Y: " + to_string(static_cast<int>(mousePoint.y)), XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f), 20);
		_fontEngine->Update(XMFLOAT2(10, 35), "Impact", "FPS: " + to_string(_framesPerSecond->GetFramesPerSeond()), XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f), 20);
		_fontEngine->Update(XMFLOAT2(10, 60), "Impact", "Cpu: " + to_string(_cpu->GetCpuPercentage()) + "%", XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f), 20);

		_direct3D->TurnZBufferOn();
		 
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