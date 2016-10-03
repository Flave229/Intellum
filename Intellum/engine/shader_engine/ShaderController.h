#ifndef _SHADERCONTROLLER_H_
#define _SHADERCONTROLLER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

#include "../../error_handling/Exception.h"
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
	IShaderType* _defaultShader;
	IShaderType* _fontShader;

public:
	ShaderController();
	ShaderController(const ShaderController&);
	~ShaderController();

	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();

	IShaderType* GetShader(ShaderType type);
};

#endif