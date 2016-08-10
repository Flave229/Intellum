#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "directX3D.h"

#include "Camera.h"
#include "Model.h"
#include "ShaderController.h"
#include "Light.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
// Member Level Variables
private:
	DirectX3D* _direct3D;
	
	Camera* _camera;
	Model* _model;
	ShaderController* _shader;
	Light* _light;

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