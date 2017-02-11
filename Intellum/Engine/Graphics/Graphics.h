#pragma once

#include "../Camera/Camera.h"
#include "../ShaderEngine/ShaderController.h"
#include "../ShaderEngine/IShaderType.h"
#include "../ShaderEngine/DefaultShader.h"
#include "Light.h"
#include "Bitmap.h"
#include "../../ErrorHandling/Exception.h"
#include "../DirectX3D.h"
#include "../FontEngine/FontEngine.h"
#include "../SystemMetrics/FramesPerSecond.h"
#include "../SystemMetrics/Cpu.h"
#include "../Object/SceneObject.h"
#include "../Handlers/ObjectHandler.h"
#include "../../Common/Constants.h"

class Graphics
{
private:
	DirectX3D* _direct3D;

	FontEngine* _fontEngine;
	FramesPerSecond* _framesPerSecond;
	Cpu* _cpu;

	ShaderController* _shaderController;
	ObjectHandler* _objectHandler;

	Camera* _camera;
	Light* _light;
	Bitmap* _bitmap;

private:
	void Initialise(Input* input, Box screenSize, HWND hwnd);
public:
	Graphics(Input* input, Box screenSize, HWND hwnd, FramesPerSecond* framesPerSecond, Cpu* cpu);
	~Graphics();

	void Shutdown();
	void Update(float delta) const;
	void Render(XMFLOAT2 mousePoint) const;
};