#pragma once

#include "IShaderType.h"

using namespace std;

class FontShader : public IShaderType
{
	struct ConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct CameraBuffer
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct ColorBuffer
	{
		float colorOverloadEnabled;
		XMFLOAT3 padding;
		XMFLOAT4 colorOverload;
	};

private:
	ID3D11Buffer* _colorBuffer;

	bool _colorOverloadEnabled;
	XMFLOAT4 _fontColor;
public:
	FontShader(DirectX3D* direct3D, Camera* camera, Light* light);
	~FontShader();

	bool Initialise(HWND hwnd) override;
	bool InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) override;
	void Shutdown() override;

	bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture) override;

	bool Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture) override;
	void RenderShader(int indexCount) override;

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) override;

	void SetColorOverload(bool state, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
};