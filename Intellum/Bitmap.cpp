#include "Bitmap.h"

Bitmap::Bitmap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX3D* direct3D): _vertexBuffer(nullptr), _indexBuffer(nullptr), _vertexCount(0), _indexCount(0), _screenWidth(0), _screenHeight(0), _bitmapWidth(0), _bitmapHeight(0), _previousPosX(0), _previousPosY(0), _texture(nullptr), _shader(new DefaultShader(direct3D, device, deviceContext)), _device(device), _deviceContext(deviceContext)
{
}

Bitmap::Bitmap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IShaderType* shader) : _vertexBuffer(nullptr), _indexBuffer(nullptr), _vertexCount(0), _indexCount(0), _screenWidth(0), _screenHeight(0), _bitmapWidth(0), _bitmapHeight(0), _previousPosX(0), _previousPosY(0), _texture(nullptr), _shader(shader), _device(device), _deviceContext(deviceContext)
{
	
}

Bitmap::Bitmap(const Bitmap& other): _vertexBuffer(other._vertexBuffer), _indexBuffer(other._indexBuffer), _vertexCount(other._vertexCount), _indexCount(other._indexCount), _screenWidth(other._screenWidth), _screenHeight(other._screenHeight), _bitmapWidth(other._bitmapWidth), _bitmapHeight(other._bitmapHeight), _previousPosX(other._previousPosX), _previousPosY(other._previousPosY), _texture(other._texture), _shader(other._shader), _device(other._device), _deviceContext(other._deviceContext)
{
}


Bitmap::~Bitmap()
{
}

bool Bitmap::Initialise(int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight, char* textureFilename)
{
	bool result;

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;

	_previousPosX = -1;
	_previousPosY = -1;

	result = InitialiseBuffers();
	if (!result) return false;

	result = LoadTexture(textureFilename);
	if (!result) return false;

	return true;
}

void Bitmap::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

bool Bitmap::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, Light* light, int positionX, int positionY)
{
	bool result = UpdateBuffers(positionX, positionY);
	if (!result) return false;

	RenderBuffers();

	result = _shader->Render(GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, GetTexture(), cameraPosition, light);
	if (!result) return false;

	return true;
}

int Bitmap::GetIndexCount()
{
	return _indexCount;
}

ID3D11ShaderResourceView* Bitmap::GetTexture()
{
	return _texture->GetTexture();
}

bool Bitmap::InitialiseBuffers()
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	_vertexCount = 6;
	_indexCount = _vertexCount;

	vertices = new Vertex[_vertexCount];
	if (!vertices) return false;

	indices = new unsigned long[_indexCount];
	if (!indices) return false;

	memset(vertices, 0, sizeof(Vertex) * _vertexCount);

	for (int i = 0; i < _indexCount; i++)
	{
		indices[i] = i;
	}

	// Setup Vertex Buffer Description
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result)) return false;

	// Setup Index Buffer Description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = _device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result)) return false;

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void Bitmap::ShutdownBuffers()
{
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}
}

bool Bitmap::UpdateBuffers(int positionX, int positionY)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if ((positionX == _previousPosX) && (positionY == _previousPosY))
	{
		return true;
	}

	_previousPosX = positionX;
	_previousPosY = positionY;

	float left = static_cast<float>((_screenWidth / 2) * -1) + static_cast<float>(positionX);
	float right = left + static_cast<float>(_bitmapWidth);

	float top = static_cast<float>(_screenHeight / 2) - static_cast<float>(positionY);
	float bottom = top - static_cast<float>(_bitmapHeight);

	Vertex* vertices = new Vertex[_vertexCount];
	if (!vertices) return false;

	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(left, top, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	result = _deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	Vertex* verticesPtr = static_cast<Vertex*>(mappedResource.pData);

	memcpy(verticesPtr, static_cast<void*>(vertices), (sizeof(Vertex) * _vertexCount));

	_deviceContext->Unmap(_vertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

void Bitmap::RenderBuffers()
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Bitmap::LoadTexture(char* filename)
{
	_texture = new Texture(_device, _deviceContext);
	if (!_texture) return false;

	return _texture->Initialise(filename);
}

void Bitmap::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = nullptr;
	}
}
