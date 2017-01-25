#include "Model.h"
#include "../../Loaders/OBJLoader.h"

Model::Model(DirectX3D* direct3D, IShaderType* shader, char* textureFilename, char* modelFilename) : _direct3D(direct3D), _geometry(new Geometry), _texture(nullptr), _shader(shader)
{
	Initialise(textureFilename, modelFilename);
}

Model::Model(const Model& other) : _direct3D(other._direct3D), _geometry(other._geometry), _texture(other._texture), _shader(other._shader)
{
}

Model::~Model()
{
}

void Model::Initialise(char* textureFilename, char* modelFilename)
{
	LoadModel(modelFilename);
	LoadTexture(textureFilename);
}

void Model::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void Model::Render(float delta, Light* light)
{
	RenderBuffers();

	XMMATRIX worldMatrix;
	XMMATRIX projectionMatrix;
	_direct3D->MapWorldMatrixInto(worldMatrix);
	_direct3D->MapProjectionMatrixInto(projectionMatrix);

	static float rotation = 0.0f;

	rotation += static_cast<float>(XM_PI) * 0.5f * delta;

	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	worldMatrix *= XMMatrixRotationY(rotation);

	_shader->Render(GetIndexCount(), worldMatrix, projectionMatrix, GetTexture(), light);
}

int Model::GetIndexCount()
{
	return _geometry->IndexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return _texture->GetTexture();
}

void Model::ShutdownBuffers()
{
	if(_geometry)
	{
		_geometry->Shutdown();
		delete _geometry;
		_geometry = nullptr;
	}
}

void Model::RenderBuffers()
{
	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &_geometry->VBStride, &_geometry->VBOffset);
	deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::LoadTexture(char* filename)
{
	_texture = new Texture(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), filename);
	if (!_texture)
		throw Exception("Failed to load the texture at: '" + string(filename) + "'");
}

void Model::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = nullptr;
	}
}

void Model::LoadModel(char* filename)
{
	*_geometry = OBJLoader::Load(filename, _direct3D->GetDevice());
	if (!_geometry)
		throw Exception("Failed to load the model at: '" + string(filename) + "'");
}