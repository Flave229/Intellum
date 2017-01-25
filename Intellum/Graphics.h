#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Camera.h"
#include "Model.h"
#include "Engine\ShaderEngine\ShaderController.h"
#include "Engine\ShaderEngine\IShaderType.h"
#include "Engine\ShaderEngine\DefaultShader.h"
#include "Light.h"
#include "Bitmap.h"
#include "ErrorHandling/Exception.h"
#include "Engine/DirectX3D.h"
#include "Engine/FontEngine/FontEngine.h"
#include "Engine/SystemMetrics/FramesPerSecond.h"
#include "Engine/SystemMetrics/Cpu.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
private:
	DirectX3D* _direct3D;

	FontEngine* _fontEngine;
	FramesPerSecond* _framesPerSecond;
	Cpu* _cpu;
	
	Camera* _camera;
	Model* _model;
	ShaderController* _shaderController;
	Light* _light;
	Bitmap* _bitmap;

private:
	void Initialise(Box screenSize, HWND hwnd);

	bool Render(float delta, XMFLOAT2 mousePoint);
public:
	Graphics(Box screenSize, HWND hwnd, FramesPerSecond* framesPerSecond, Cpu* cpu);
	Graphics(const Graphics& other);
	~Graphics();

	void Shutdown();
	bool Frame(float delta, XMFLOAT2 mousePoint);
};

#endif