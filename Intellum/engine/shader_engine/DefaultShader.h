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
	string _vertexShaderFile;
	string _pixelShaderFile;

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;

	ID3D11Buffer* _matrixBuffer;
	ID3D11Buffer* _cameraBuffer;
	ID3D11Buffer* _lightBuffer;
public:
	DefaultShader(string vertexShader, string pixelShader);
	~DefaultShader();

	virtual bool InitialiseShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	virtual void ShutdownShader();

	virtual bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor,
		XMFLOAT4 diffuseColor, XMFLOAT4 specularColor, float specularPower);

	virtual bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT3 cameraPosition,
		XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT4 specularColor, float specularPower);

	virtual void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName);
};

