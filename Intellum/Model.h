#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include <fstream>

#include "Texture.h"
#include "common/Geometry.h"
#include "engine\shader_engine\IShaderType.h"
#include "engine\shader_engine\DefaultShader.h"

using namespace DirectX;
using namespace std;

class Model
{
// Member Level Variables
private:
	Geometry* _geometry;
	Texture* _texture;
	IShaderType* _shader;

// Function Declarations
private:
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(ID3D11Device*, char*);
public:
	Model();
	Model(IShaderType* shader);
	Model(const Model&);
	~Model();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
};

#endif