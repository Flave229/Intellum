#pragma once

#include "IShaderType.h"

using namespace std;

class UIShader : public IShaderType
{
public:
	UIShader(DirectX3D* direct3D, Camera* camera, Light* light);
	~UIShader();

	void Initialise(HWND hwnd) override;
	void InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) override;
	void Shutdown() override;

	void SetShaderParameters(ShaderResources shaderResources) override;

	void Render(int indexCount, ShaderResources shaderResources) override;
	void RenderShader(int indexCount) override;

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) override;
};