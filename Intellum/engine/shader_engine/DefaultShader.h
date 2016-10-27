#pragma once

#include "IShaderType.h"

using namespace std;

class DefaultShader : public IShaderType
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

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};

private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

public:
	DefaultShader(DirectX3D* _direct3D, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DefaultShader();

	virtual bool Initialise(HWND hwnd);
	virtual bool InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	virtual void Shutdown();

	virtual bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light);

	virtual bool Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light);

	virtual void RenderShader(int indexCount);

	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName);
};