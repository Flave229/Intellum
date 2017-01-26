#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include <fstream>

#include "../../Common/Geometry.h"
#include "../../Engine/DirectX3D.h"
#include "../Graphics/Texture.h"
#include "../../Loaders/OBJLoader.h"

using namespace DirectX;
using namespace std;

class Appearance
{
// Member Level Variables
private:
	DirectX3D* _direct3D;

	Geometry* _geometry;
	Texture* _texture;

// Function Declarations
private:
	void Initialise(char* textureFilename, char* modelFilename);

	void ShutdownBuffers();
	void RenderBuffers() const;

	void LoadTexture(char* filename);
	void ReleaseTexture();

	void LoadModel(char* filename) const;
public:
	Appearance(DirectX3D* direct3D, char* textureFilename, char* modelFilename);
	Appearance(const Appearance& other);
	~Appearance();

	void Shutdown();
	void Render() const;

	int GetIndexCount() const;

	ID3D11ShaderResourceView* GetTexture() const;
};

#endif