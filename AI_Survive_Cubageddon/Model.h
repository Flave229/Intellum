#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include <fstream>

#include "Texture.h"

using namespace DirectX;
using namespace std;

class Model
{
// Structures
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

// Member Level Variables
private:
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	int _vertexCount;
	int _indexCount;

	Texture* _texture;
	ModelType* _modelType;

// Function Declarations
private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
};

#endif