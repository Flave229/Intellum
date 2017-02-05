#include "Appearance.h"

Appearance::Appearance(DirectX3D* direct3D, vector<char*> textureFilenames, char* modelFilename) : _direct3D(direct3D), _geometry(new Geometry), _texture(nullptr)
{
	Initialise(textureFilenames, modelFilename);
}

Appearance::Appearance(const Appearance& other) : _direct3D(other._direct3D), _geometry(other._geometry), _texture(other._texture)
{
}

Appearance::~Appearance()
{
}

void Appearance::Initialise(vector<char*> textureFilenames, char* modelFilename)
{
	LoadModel(modelFilename);
	LoadTextures(textureFilenames);
}

void Appearance::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void Appearance::Render() const
{
	RenderBuffers();
}

int Appearance::GetIndexCount() const
{
	return _geometry->IndexCount;
}

Texture* Appearance::GetTexture() const
{
	return _texture;
}

void Appearance::ShutdownBuffers()
{
	if(_geometry)
	{
		_geometry->Shutdown();
		delete _geometry;
		_geometry = nullptr;
	}
}

void Appearance::RenderBuffers() const
{
	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &_geometry->VBStride, &_geometry->VBOffset);
	deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Appearance::LoadTextures(vector<char*> filenames)
{
	_texture = new Texture(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), filenames);

	if (!_texture)
	{
		string message = "Failed to load one or all of the following textures: '";
		for (int i = 0; i < filenames.size(); i++)
		{
			message += "\t'" + string(filenames.at(i)) + "\n'";
		}
		throw Exception(message);
	}
}

void Appearance::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = nullptr;
	}
}

void Appearance::LoadModel(char* filename) const
{
	*_geometry = OBJLoader::Load(filename, _direct3D->GetDevice());
	if (!_geometry)
		throw Exception("Failed to load the model at: '" + string(filename) + "'");
}
