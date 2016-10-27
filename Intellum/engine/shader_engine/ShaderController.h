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

	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	IShaderType* _defaultShader;
	IShaderType* _fontShader;

public:
	ShaderController(DirectX3D* direct3D, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	ShaderController(const ShaderController&);
	~ShaderController();

	bool Initialise(HWND);
	void Shutdown();

	IShaderType* GetShader(ShaderType type);
};

#endif