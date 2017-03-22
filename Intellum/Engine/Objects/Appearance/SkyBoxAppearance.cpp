#include "SkyBoxAppearance.h"
#include "../../../loaders/OBJLoader.h"

SkyBoxAppearance::SkyBoxAppearance(DirectX3D* direct3D, vector<char*> textureFiles, char* modelFilename): _direct3D(direct3D), _geometry(new Geometry)
{
	Initialise(textureFiles, modelFilename);
}

void SkyBoxAppearance::Shutdown()
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
}

void SkyBoxAppearance::Update(XMFLOAT2 position, Box bitmapSize)
{
	throw Exception("Update method not implemented in the Skybox Appearance Class");
}

void SkyBoxAppearance::Render() const
{
	_direct3D->GetRasterizer()->SetRasterizerCullMode(D3D11_CULL_NONE);

	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &_geometry->VBStride, &_geometry->VBOffset);
	deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void SkyBoxAppearance::Initialise(vector<char*> textureFiles, char* modelFilename)
{
	LoadModel(modelFilename);
	LoadTextures(textureFiles);
}

void SkyBoxAppearance::LoadModel(char* filename) const
{
	*_geometry = OBJLoader::Load(filename, _direct3D->GetDevice());
	if (!_geometry)
		throw Exception("Failed to load the model at: '" + string(filename) + "'");
}

void SkyBoxAppearance::LoadTextures(vector<char*> textureFiles)
{
	_textures = CreateTexture::ListFrom(_direct3D, textureFiles);
}

int SkyBoxAppearance::GetIndexCount() const
{
	return _geometry->IndexCount;
}

vector<ID3D11ShaderResourceView*> SkyBoxAppearance::GetTextures() const
{
	vector<ID3D11ShaderResourceView*> textureViews;

	for (Texture* texture : _textures)
	{
		textureViews.push_back(texture->GetTexture());
	}

	return textureViews;
}

ID3D11ShaderResourceView* SkyBoxAppearance::GetLightMap() const
{
	return nullptr;
}