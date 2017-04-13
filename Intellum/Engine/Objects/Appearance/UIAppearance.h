#pragma once
#include "../../../common/Vertex.h"
#include "IAppearance.h"

class UIAppearance : public IAppearance
{
private:
	DirectX3D* _direct3D;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	int _vertexCount;
	int _indexCount;

	Box _screenSize;
	Box _bitmapSize;
	XMFLOAT2 _previousPosition;

	vector<Texture*> _textures;
	Texture* _lightMap;

private:
	void Initialise(vector<char*> textureFiles, char* lightMapFile);
	void InitialiseBuffers();
	void LoadTextures(vector<char*> filenames, char* lightMapFile);

public:
	UIAppearance(DirectX3D* direct3D, Box screenSize, Box bitmapSize, vector<char*> textureFiles, char* lightMapFile);
	~UIAppearance() override = default;
	
	void Shutdown() override;
	void Render() const override;

	void Update(XMFLOAT2 position, Box bitmapSize) override;
	
	int GetIndexCount() const override;
	Texture* GetTexture() const override;
	vector<ID3D11ShaderResourceView*> GetTextures() const override;
	ID3D11ShaderResourceView* GetLightMap() const override;
};