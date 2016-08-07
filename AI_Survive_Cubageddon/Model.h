#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include "Texture.h"

using namespace DirectX;

class Model
{
// Structures
	struct VertexType
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

	Texture* _texture;

// Function Declarations
private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();
public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
};

#endif