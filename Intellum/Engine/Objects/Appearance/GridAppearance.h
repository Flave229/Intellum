#pragma once
#include "IAppearance.h"
#include "../../../common/Geometry.h"

class GridAppearance : public IAppearance
{
private:
	DirectX3D* _direct3D;
	Geometry* _geometry;
	vector<Texture*> _textures;
	Texture* _lightMap;

private:
	void Initialise(vector<char*> textureFiles, char* lightMapFile);
	void LoadTextures(vector<char*> textureFiles, char* lightMapFile) override;
	void GenerateModel();

public:
	GridAppearance(DirectX3D* direct3D, vector<char*> textureFiles, char* lightMapFile);
	~GridAppearance() override = default;

	void Shutdown() override;
	void Render() const override;
	void Update(XMFLOAT2 position, Box bitmapSize) override;

	int GetIndexCount() const override;
	vector<ID3D11ShaderResourceView*> GetTextures() const override;
	ID3D11ShaderResourceView* GetLightMap() const override;
};

