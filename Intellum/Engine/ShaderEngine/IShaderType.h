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
#include "../Camera/Camera.h"
#include "ConstantBuffers/MatrixBuffer.h"
#include "ConstantBuffers/CameraBuffer.h"
#include "ConstantBuffers/LightBuffer.h"
#include "ConstantBuffers/ColorOverrideBuffer.h"
#include "ConstantBuffers/TextureBuffer.h"
#include "ShaderParameters/ShaderParameterConstructor.h"

using namespace DirectX;

class IShaderType
{
protected:
	DirectX3D* _direct3D;
	Camera* _camera;
	Light* _light;

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;

	IShaderBuffer* _matrixBuffer;
	IShaderBuffer* _cameraBuffer;
	IShaderBuffer* _lightBuffer;
	IShaderBuffer* _colorBuffer;
	IShaderBuffer* _textureBuffer;

public:
	IShaderType(DirectX3D* direct3D, Camera* camera, Light* light) : _direct3D(direct3D), _camera(camera), _light(light) {};
	virtual ~IShaderType() {};

	virtual void Initialise(HWND hwnd) = 0;
	virtual void InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) = 0;
	virtual void Shutdown() = 0;

	virtual void SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, vector<ID3D11ShaderResourceView*> textureArray, int textureCount) = 0;

	virtual void Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, vector<ID3D11ShaderResourceView*> textureArray, int textureCount) = 0;

	virtual void RenderShader(int indexCount) = 0;
	
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) = 0;
};
