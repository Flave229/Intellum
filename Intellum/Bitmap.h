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

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	int _vertexCount;
	int _indexCount;

	Box _screenSize;
	Box _bitmapSize;
	XMFLOAT2 _previousPosition;

	IShaderType* _shader;
	Texture* _texture;

// Function Declarations
private:
	void Initialise(Box screenSize, Box bitmapBox, char* textureFilename);
	void InitialiseBuffers();
	void LoadTexture(char*);

	bool UpdateBuffers(XMFLOAT2 position, Box bitmapSize);
	void RenderBuffers();

	void ShutdownBuffers();
	void ReleaseTexture();
public:
	Bitmap(DirectX3D* direct3D, IShaderType* shader, Box screenSize, Box bitmapBox, char* textureFilename);
	Bitmap(const Bitmap&);
	~Bitmap();

	void Shutdown();
	bool Render(Light* light, XMFLOAT2 position, Box bitmapSize);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
};