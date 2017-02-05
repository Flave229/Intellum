#pragma once

#include "IShaderType.h"

using namespace std;

class FontShader : public IShaderType
{
	struct ColorBuffer
	{
		float colorOverloadEnabled;
		XMFLOAT3 padding;
		XMFLOAT4 colorOverload;
	};

	struct TextureBuffer
	{
		float texturesIncluded;
		XMFLOAT3 padding;
	};

private:
	ID3D11Buffer* _colorBuffer;
	XMMATRIX _viewMatrix;

	bool _colorOverloadEnabled;
	XMFLOAT4 _fontColor;

private:
	void SetColorBuffer(unsigned bufferNumber) const;
	void SetTextureBuffer(unsigned bufferNumber, int textureCount) const;

public:
	FontShader(DirectX3D* direct3D, Camera* camera, Light* light);
	~FontShader();

	void Initialise(HWND hwnd) override;
	void InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) override;
	void Shutdown() override;

	void SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray, int textureCount) override;

	void Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray, int textureCount) override;
	void RenderShader(int indexCount) override;

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) override;

	void SetColorOverload(bool state, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
};