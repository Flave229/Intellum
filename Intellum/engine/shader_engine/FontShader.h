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
	FontShader(DirectX3D* _direct3D);
	~FontShader();

	virtual bool Initialise(HWND hwnd);
	virtual bool InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	virtual void Shutdown();

	virtual bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light);

	virtual bool Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light);

	virtual void RenderShader(int indexCount);

	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName);

	void SetColorOverload(bool state, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
};