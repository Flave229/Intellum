#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include <fstream>

#include "Texture.h"
#include "common/Geometry.h"
#include "engine\shader_engine\IShaderType.h"
#include "engine\shader_engine\DefaultShader.h"
#include "engine\DirectX3D.h"
#include "Camera.h"

using namespace DirectX;
using namespace std;

class Model
{
// Member Level Variables
private:
	Camera* _camera;
	DirectX3D* _direct3D;

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
	Model(DirectX3D* direct3D, Camera* camera);
	Model(DirectX3D* direct3D, Camera* camera, IShaderType* shader);
	Model(const Model&);
	~Model();

	bool Initialise(char*, char*);
	void Shutdown();
	void Render(float delta, Light* light);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
};

#endif