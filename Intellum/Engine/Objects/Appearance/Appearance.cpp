#include "Appearance.h"

Appearance::Appearance(DirectX3D* direct3D, vector<char*> textureFilenames, char* lightMapFile, char* modelFilename) : _direct3D(direct3D), _geometry(new Geometry)
{
	Initialise(textureFilenames, lightMapFile, modelFilename);
}

void Appearance::Initialise(vector<char*> textureFilenames, char* lightMapFile, char* modelFilename)
{
	LoadModel(modelFilename);
	LoadTextures(textureFilenames, lightMapFile);
}

void Appearance::Shutdown()
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

void Appearance::Render() const
{
	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &_geometry->VBStride, &_geometry->VBOffset);
	deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Appearance::Update(XMFLOAT2 position, Box bitmapSize)
{
	throw Exception("Update method not implemented in the Appearance Class");
}

void Appearance::LoadTextures(vector<char*> textureFiles, char* lightMapFile)
{
	_textures = CreateTexture::ListFrom(_direct3D, textureFiles);
	_lightMap = CreateTexture::From(_direct3D, lightMapFile);
}

void Appearance::LoadModel(char* filename) const
{
	*_geometry = OBJLoader::Load(filename, _direct3D->GetDevice());
	if (!_geometry)
		throw Exception("Failed to load the model at: '" + string(filename) + "'");
}

int Appearance::GetIndexCount() const
{
	return _geometry->IndexCount;
}

vector<ID3D11ShaderResourceView*> Appearance::GetTextures() const
{
	vector<ID3D11ShaderResourceView*> textureViews;

	for (Texture* texture : _textures)
	{
		textureViews.push_back(texture->GetTexture());
	}

	return textureViews;
}

ID3D11ShaderResourceView* Appearance::GetLightMap() const
{
	if (_lightMap != nullptr)
		return _lightMap->GetTexture();

	return nullptr;
}