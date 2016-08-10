#include "Model.h"

Model::Model(): _geometry(new Geometry), _texture(nullptr), _modelType(nullptr)
{
}

Model::Model(const Model& other) : _geometry(other._geometry), _texture(other._texture), _modelType(other._modelType)
{
}

Model::~Model()
{
}

bool Model::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result) return false;

	result = InitialiseBuffers(device);
	if (!result) return false;

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result) return false;

	return true;
}

void Model::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int Model::GetIndexCount()
{
	return _geometry->IndexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return _texture->GetTexture();
}

bool Model::InitialiseBuffers(ID3D11Device* device)
{
	HRESULT result;
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	vertices = new VertexType[_geometry->VertexCount];
	if (!vertices) return false;

	indices = new unsigned long[_geometry->IndexCount];
	if (!indices) return false;

	// Load vertex and index array
	for (int i = 0; i < _geometry->VertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(_modelType[i].x, _modelType[i].y, _modelType[i].z);
		vertices[i].texture = XMFLOAT2(_modelType[i].tu, _modelType[i].tv);
		vertices[i].normal = XMFLOAT3(_modelType[i].nx, _modelType[i].ny, _modelType[i].nz);

		indices[i] = i;
	}

	// Setup description for the Vertex Buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _geometry->VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_geometry->VertexBuffer);
	if (FAILED(result)) return false;

	// Setup description for the Index Buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _geometry->IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_geometry->IndexBuffer);
	if (FAILED(result)) return false;

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
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
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
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

bool Model::LoadModel(char* fileName)
{
	ifstream fin;
	char input;

	fin.open(fileName);
	if (fin.fail()) return false;

	fin.get(input);

	while(input != ':')
	{
		fin.get(input);
	}

	fin >> _geometry->VertexCount;
	_geometry->IndexCount = _geometry->VertexCount;

	_modelType = new ModelType[_geometry->VertexCount];
	if (!_modelType) return false;

	fin.get(input);
	
	while(input != ':')
	{
		fin.get(input);
	}
	
	fin.get(input);
	fin.get(input);

	for (int i = 0; i < _geometry->VertexCount; i++)
	{
		fin >> _modelType[i].x >> _modelType[i].y >> _modelType[i].z;
		fin >> _modelType[i].tu >> _modelType[i].tv;
		fin >> _modelType[i].nx >> _modelType[i].ny >> _modelType[i].nz;
	}

	fin.close();

	return true;
}

void Model::ReleaseModel()
{
	if (_modelType)
	{
		delete[] _modelType;
		_modelType = nullptr;
	}
}
