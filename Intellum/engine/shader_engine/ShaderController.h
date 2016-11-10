#ifndef _SHADERCONTROLLER_H_
#define _SHADERCONTROLLER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

#include "../../error_handling/Exception.h"
#include "../DirectX3D.h"
#include "DefaultShader.h"
#include "FontShader.h"
#include "IShaderType.h"

using namespace DirectX;
using namespace std;

enum ShaderType
{
	SHADER_DEFAULT,
	SHADER_FONT
};

class ShaderController
{
private:
	DirectX3D* _direct3D;

	IShaderType* _defaultShader;
	IShaderType* _fontShader;

public:
	ShaderController(DirectX3D* direct3D);
	ShaderController(const ShaderController&);
	~ShaderController();

	// TODO: inject camera in constructor rather than passing to initialise
	bool Initialise(HWND hwnd, Camera* camera);
	void Shutdown();

	IShaderType* GetShader(ShaderType type);
};

#endif