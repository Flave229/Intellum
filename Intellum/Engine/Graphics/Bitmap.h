#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../../Engine/ShaderEngine/IShaderType.h"
#include "../../Engine/ShaderEngine/DefaultShader.h"
#include "../../Engine/DirectX3D.h"
#include "Texture.h"
#include "Light.h"
#include "../Object/Appearance.h"

using namespace DirectX;

class Bitmap
{
private:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

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
	vector<Texture*> _textures;

private:
	void Initialise(Box screenSize, Box bitmapBox, vector<char*> textureFilenames);
	void InitialiseBuffers();
	void LoadTextures(vector<char*> filenames);

	void UpdateBuffers(XMFLOAT2 position, Box bitmapSize);
	void RenderBuffers() const;

	void ShutdownBuffers();
	void ReleaseTexture();
public:
	Bitmap(DirectX3D* direct3D, IShaderType* shader, Box screenSize, Box bitmapBox, vector<char*> textureFilenames);
	~Bitmap();

	void Shutdown();

	void Update(XMFLOAT2 position, Box bitmapSize);
	void Render();

	int GetIndexCount() const;
	vector<ID3D11ShaderResourceView*> GetTextures() const;
};