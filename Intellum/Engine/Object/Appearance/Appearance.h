#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "../../../loaders/OBJLoader.h"
#include "../../DirectX3D.h"
#include "../../../common/Geometry.h"
#include "IAppearance.h"

using namespace DirectX;
using namespace std;

class Appearance : public IAppearance
{
private:
	DirectX3D* _direct3D;

	Geometry* _geometry;
	vector<Texture*> _textures;
	Texture* _lightMap;

private:
	void Initialise(vector<char*> textureFiles, char* lightMapFile, char* modelFilename);
	void LoadTextures(vector<char*> textureFiles, char* lightMapFile) override;
	void LoadModel(char* filename) const;

public:
	Appearance(DirectX3D* direct3D, vector<char*> textureFiles, char* lightMapFile, char* modelFilename);
	~Appearance() override = default;

	void Shutdown() override;
	void Render() const override;

	void Update(XMFLOAT2 position, Box bitmapSize) override;

	int GetIndexCount() const override;
	vector<ID3D11ShaderResourceView*> GetTextures() const override;
};