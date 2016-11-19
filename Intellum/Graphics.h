#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Camera.h"
#include "Model.h"
#include "engine\shader_engine\ShaderController.h"
#include "engine\shader_engine\IShaderType.h"
#include "engine\shader_engine\DefaultShader.h"
#include "Light.h"
#include "Bitmap.h"
#include "error_handling/Exception.h"
#include "engine/DirectX3D.h"
#include "engine/font_engine/FontEngine.h"
#include "engine/system_metrics/FramesPerSecond.h"

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
	
	Camera* _camera;
	Model* _model;
	ShaderController* _shaderController;
	Light* _light;
	Bitmap* _bitmap;

private:
	void Initialise(Box screenSize, HWND hwnd);

	bool Render(float delta, XMFLOAT2 mousePoint);
public:
	Graphics(Box screenSize, HWND hwnd, FramesPerSecond* framesPerSecond);
	Graphics(const Graphics& other);
	~Graphics();

	void Shutdown();
	bool Frame(float delta, XMFLOAT2 mousePoint);
};

#endif