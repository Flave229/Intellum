#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Model
{
// Member Level Variables
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	int _vertexCount;
	int _indexCount;

// Function Declarations
private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialise(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
};

#endif