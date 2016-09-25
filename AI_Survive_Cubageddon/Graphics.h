#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Camera.h"
#include "Model.h"
#include "ShaderController.h"
#include "Light.h"
#include "Bitmap.h"
#include "error_handling/Exception.h"
#include "engine/DirectX3D.h"
#include "engine/font_engine/FontEngine.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
// Member Level Variables
private:
	DirectX3D* _direct3D;

	FontEngine* _fontEngine;
	
	Camera* _camera;
	Model* _model;
	ShaderController* _shader;
	Light* _light;
	Bitmap* _bitmap;

// Function Declarations
private:
	bool Render(float);
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Frame(float);
};

#endif