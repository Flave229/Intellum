#include "GridAppearance.h"

GridAppearance::GridAppearance(DirectX3D* direct3D, vector<char*> textureFiles, char* lightMapFile) : _direct3D(direct3D), _geometry(new Geometry)
{
	Initialise(textureFiles, lightMapFile);
}

void GridAppearance::Shutdown()
{
	for (unsigned long long i = _textures.size(); i > 0; i--)
	{
		_textures.back()->Shutdown();
		_textures.pop_back();
	}

	_textures.clear();

	if (_geometry)
	{
		_geometry->Shutdown();
		delete _geometry;
		_geometry = nullptr;
	}

	if (_lightMap)
	{
		_lightMap->Shutdown();
		delete _lightMap;
		_lightMap = nullptr;
	}
}

void GridAppearance::Initialise(vector<char*> textureFilenames, char* lightMapFile)
{
	GenerateModel();
	LoadTextures(textureFilenames, lightMapFile);
}


void GridAppearance::LoadTextures(vector<char*> textureFiles, char* lightMapFile)
{
	_textures = CreateTexture::ListFrom(_direct3D, textureFiles);
	_lightMap = CreateTexture::From(_direct3D, lightMapFile);
}

void GridAppearance::GenerateModel()
{
	throw Exception("Not Implemented");
}

void GridAppearance::Render() const
{
}

void GridAppearance::Update(XMFLOAT2 position, Box bitmapSize)
{
}

int GridAppearance::GetIndexCount() const
{
	return _geometry->IndexCount;
}

vector<ID3D11ShaderResourceView*> GridAppearance::GetTextures() const
{
	vector<ID3D11ShaderResourceView*> textureViews;

	for (Texture* texture : _textures)
	{
		textureViews.push_back(texture->GetTexture());
	}

	return textureViews;
}

ID3D11ShaderResourceView* GridAppearance::GetLightMap() const
{
	if (_lightMap != nullptr)
		return _lightMap->GetTexture();

	return nullptr;
}
