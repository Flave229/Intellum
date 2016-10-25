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
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	Geometry* _geometry;
	Texture* _texture;
	IShaderType* _shader;

// Function Declarations
private:
	void ShutdownBuffers();
	void RenderBuffers();

	bool LoadTexture(char*);
	void ReleaseTexture();

	bool LoadModel(char*);
public:
	Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IShaderType* shader);
	Model(const Model&);
	~Model();

	bool Initialise(char*, char*);
	void Shutdown();
	void Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
};

#endif