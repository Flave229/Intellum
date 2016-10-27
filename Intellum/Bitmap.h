#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "engine\shader_engine\IShaderType.h"
#include "engine\shader_engine\DefaultShader.h"
#include "engine\DirectX3D.h"
#include "Texture.h"
#include "Light.h"

using namespace DirectX;

class Bitmap
{
private:
// Structures
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

// Member Level Variables
private:
	DirectX3D* _direct3D;

	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	int _vertexCount;
	int _indexCount;

	int _screenWidth;
	int _screenHeight;
	int _bitmapWidth;
	int _bitmapHeight;
	int _previousPosX;
	int _previousPosY;

	IShaderType* _shader;
	Texture* _texture;

// Function Declarations
private:
	bool InitialiseBuffers();
	void ShutdownBuffers();
	bool UpdateBuffers(int, int);
	void RenderBuffers();

	bool LoadTexture(char*);
	void ReleaseTexture();
public:
	Bitmap(DirectX3D* direct3D, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Bitmap(DirectX3D* direct3D, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IShaderType* shader);
	Bitmap(const Bitmap&);
	~Bitmap();

	bool Initialise(int, int, int, int, char*);
	void Shutdown();
	bool Render(XMMATRIX viewMatrix, XMFLOAT3 cameraPosition, Light* light, int positionX, int positionY);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
};