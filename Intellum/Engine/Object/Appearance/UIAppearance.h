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

	Box* _screenSize;
	Box* _bitmapSize;
	XMFLOAT2* _previousPosition;

	vector<Texture*> _textures;

private:
	void Initialise(vector<char*> texturefilenames);
	void InitialiseBuffers();
	void LoadTextures(vector<char*> filenames) override;

public:
	UIAppearance(DirectX3D* direct3D, Box* screenSize, Box* bitmapSize, XMFLOAT2* position, vector<char*> texturefilenames);
	~UIAppearance() override = default;;
	
	void Shutdown() override;
	void Render() const override;

	void Update() override;
	
	int GetIndexCount() const override;
	int GetTextureCount() const override;
	vector<ID3D11ShaderResourceView*> GetTextures() const override;
};