#pragma once
#include "IAppearance.h"
#include "../../../common/Geometry.h"

class SkyBoxAppearance : public IAppearance
{
private:
	DirectX3D* _direct3D;

	Geometry* _geometry;
	vector<Texture*> _textures;

private:
	void Initialise(vector<char*> textureFiles, char* modelFilename);
	void LoadTextures(vector<char*> textureFiles);
	void LoadModel(char* filename) const;

public:
	SkyBoxAppearance(DirectX3D* direct3D, vector<char*> textureFiles, char* modelFilename);
	~SkyBoxAppearance() override = default;

	void Shutdown() override;
	void Update(XMFLOAT2 position, Box bitmapSize) override;
	void Render() const override;

	int GetIndexCount() const override;
	vector<ID3D11ShaderResourceView*> GetTextures() const override;
	ID3D11ShaderResourceView* GetLightMap() const override;
};