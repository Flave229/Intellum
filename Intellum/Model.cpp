#include "Model.h"
#include "loaders/OBJLoader.h"

Model::Model(): _geometry(new Geometry), _texture(nullptr), _shader(new DefaultShader)
{
}

Model::Model(IShaderType* shader) : _geometry(new Geometry), _texture(nullptr), _shader(shader)
{
	
}

Model::Model(const Model& other) : _geometry(other._geometry), _texture(other._texture), _shader(other._shader)
{
}

Model::~Model()
{
}

bool Model::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename)
{
	bool result;

	result = LoadModel(device, modelFilename);
	if (!result) return false;

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result) return false;

	return true;
}

void Model::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT3 cameraPosition,
	XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT4 specularColor, float specularPower)
{
	RenderBuffers(deviceContext);

	_shader->Render(deviceContext, indexCount, worldMatrix, viewMatrix,
		projectionMatrix, texture, lightDirection, cameraPosition,
		ambientColor, diffuseColor, specularColor, specularPower);
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

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &_geometry->VBStride, &_geometry->VBOffset);
	deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	_texture = new Texture;
	if (!_texture) return false;

	result = _texture->Initialise(device, deviceContext, filename);
	if (!result) return false;

	return true;
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

bool Model::LoadModel(ID3D11Device* device, char* fileName)
{
	*_geometry = OBJLoader::Load(fileName, device);
	if (!_geometry) return false;

	return true;
}