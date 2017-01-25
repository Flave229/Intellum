#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "../Graphics/Light.h"
#include "../DirectX3D.h"
#include "../../Camera.h"

using namespace DirectX;

class IShaderType
{
protected:
	DirectX3D* _direct3D;
	Camera* _camera;

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;

	ID3D11Buffer* _matrixBuffer;
	ID3D11Buffer* _cameraBuffer;
	ID3D11Buffer* _lightBuffer;

public:
	IShaderType(DirectX3D* direct3D, Camera* camera) : _direct3D(direct3D), _camera(camera) {};
	virtual ~IShaderType() {};

	virtual bool Initialise(HWND hwnd) = 0;
	virtual bool InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) = 0;
	virtual void Shutdown() = 0;

	virtual bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, Light* light) = 0;

	virtual bool Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, Light* light) = 0;

	virtual void RenderShader(int indexCount) = 0;
	
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) = 0;
};
