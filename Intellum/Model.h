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
	DirectX3D* _direct3D;

	Geometry* _geometry;
	Texture* _texture;
	IShaderType* _shader;

// Function Declarations
private:
	void Initialise(char* textureFilename, char* modelFilename);

	void ShutdownBuffers();
	void RenderBuffers();

	void LoadTexture(char* filename);
	void ReleaseTexture();

	void LoadModel(char* filename);
public:
	Model(DirectX3D* direct3D, IShaderType* shader, char* textureFilename, char* modelFilename);
	Model(const Model&);
	~Model();

	void Shutdown();
	void Render(float delta, Light* light);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
};

#endif