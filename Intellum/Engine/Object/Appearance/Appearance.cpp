#include "Appearance.h"

Appearance::Appearance(DirectX3D* direct3D, vector<char*> textureFilenames, char* modelFilename) : _direct3D(direct3D), _geometry(new Geometry)
{
	Initialise(textureFilenames, modelFilename);
}

void Appearance::Initialise(vector<char*> textureFilenames, char* modelFilename)
{
	LoadModel(modelFilename);
	LoadTextures(textureFilenames);
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
}

void Appearance::Render() const
{
	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &_geometry->VBStride, &_geometry->VBOffset);
	deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Appearance::Update()
{
}

void Appearance::LoadTextures(vector<char*> filenames)
{
	for (char* filename : filenames)
	{
		Texture* texture = new Texture(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), filename);

		if (texture == nullptr)
			throw Exception("Failed to load the following texture: \t'" + string(filename) + "'\n");

		_textures.push_back(texture);
	}
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

int Appearance::GetTextureCount() const
{
	return _textures.size();
}