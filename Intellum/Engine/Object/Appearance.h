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
private:
	DirectX3D* _direct3D;

	Geometry* _geometry;
	vector<Texture*> _textures;

private:
	void Initialise(vector<char*> textureFilenames, char* modelFilename);

	void LoadTextures(vector<char*> filenames);
	void LoadModel(char* filename) const;
public:
	Appearance(DirectX3D* direct3D, vector<char*> textureFilenames, char* modelFilename);
	~Appearance();

	void Shutdown();
	void Render() const;

	int GetIndexCount() const;

	vector<ID3D11ShaderResourceView*> GetTextures() const;
	int GetTextureCount() const;
};

#endif