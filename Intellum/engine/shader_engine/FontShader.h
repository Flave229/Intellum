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

private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;

	ID3D11Buffer* _matrixBuffer;
	ID3D11Buffer* _cameraBuffer;
	ID3D11Buffer* _lightBuffer;
public:
	FontShader();
	~FontShader();

	virtual bool Initialise(ID3D11Device* device, HWND hwnd);
	virtual bool InitialiseShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	virtual void Shutdown();

	virtual bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light);

	virtual bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light);

	virtual void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName);
};