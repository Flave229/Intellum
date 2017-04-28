#pragma once

#include "IShaderType.h"

using namespace std;

class UIShader : public IShaderType
{
private:
	XMMATRIX _viewMatrix;

	bool _colorOverloadEnabled;
	XMFLOAT4 _colorOverload;

public:
	UIShader(DirectX3D* direct3D, Camera* camera, Light* light);
	~UIShader();

	void Initialise(HWND hwnd) override;
	void InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) override;
	void Shutdown() override;

	void SetShaderParameters(ShaderResources shaderResources, XMMATRIX worldMatrix, XMMATRIX projectionMatrix) override;

	void Render(int indexCount, ShaderResources shaderResources, XMMATRIX worldMatrix, XMMATRIX projectionMatrix) override;
	void RenderShader(int indexCount) override;

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) override;

	void SetColorOverload(bool state, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
};