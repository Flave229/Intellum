#include "Model.h"
#include "loaders/OBJLoader.h"

Model::Model(DirectX3D* direct3D, IShaderType* shader) : _direct3D(direct3D), _geometry(new Geometry), _texture(nullptr), _shader(shader)
{
}

Model::Model(const Model& other) : _direct3D(other._direct3D), _geometry(other._geometry), _texture(other._texture), _shader(other._shader)
{
}

Model::~Model()
{
}

bool Model::Initialise(char* textureFilename, char* modelFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result) return false;

	result = LoadTexture(textureFilename);
	if (!result) return false;

	return true;
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
	_direct3D->GetWorldMatrix(worldMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

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

bool Model::LoadTexture(char* filename)
{
	_texture = new Texture(_direct3D->GetDevice(), _direct3D->GetDeviceContext());
	if (!_texture) return false;
	
	return _texture->Initialise(filename);
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

bool Model::LoadModel(char* fileName)
{
	*_geometry = OBJLoader::Load(fileName, _direct3D->GetDevice());
	if (!_geometry) return false;

	return true;
}