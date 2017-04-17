#pragma once

#include "../Camera/Camera.h"
#include "../ShaderEngine/ShaderController.h"
#include "../ShaderEngine/IShaderType.h"
#include "../ShaderEngine/DefaultShader.h"
#include "Light.h"
#include "../../ErrorHandling/Exception.h"
#include "../DirectX3D.h"
#include "../FontEngine/FontEngine.h"
#include "../SystemMetrics/Cpu.h"
#include "../Handlers/ObjectHandler.h"
#include "../../Common/Constants.h"

class Graphics
{
private:
	DirectX3D* _direct3D;

	FontEngine* _fontEngine;

	ShaderController* _shaderController;
	ObjectHandler* _objectHandler;

	Camera* _camera;
	Light* _light;

private:
	void Initialise(Input* input, FramesPerSecond* framesPerSecond, Cpu* cpu, Box screenSize, HWND hwnd);
public:
	Graphics(Input* input, Box screenSize, HWND hwnd, FramesPerSecond* framesPerSecond, Cpu* cpu);
	~Graphics();

	void Shutdown();
	void Update(float delta) const;
	void Render() const;
};