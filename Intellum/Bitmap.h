#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "Texture.h"

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

	Texture* _texture;

// Function Declarations
private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();
public:
	Bitmap();
	Bitmap(const Bitmap&);
	~Bitmap();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, int, int, int, int, char*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
};