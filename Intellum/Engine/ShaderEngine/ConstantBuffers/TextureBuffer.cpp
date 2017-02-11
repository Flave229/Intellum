#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(DirectX3D* direct3D) : IShaderBuffer(direct3D), _buffer(nullptr)
{
	TextureBuffer::Initialise();
}

TextureBuffer::~TextureBuffer()
{
}

void TextureBuffer::Shutdown()
{
	if (_buffer)
	{
		_buffer->Release();
		_buffer = nullptr;
	}
}

void TextureBuffer::Initialise()
{
	D3D11_BUFFER_DESC textureBufferDesc;
	textureBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureBufferDesc.ByteWidth = sizeof(Buffer);
	textureBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	textureBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureBufferDesc.MiscFlags = 0;
	textureBufferDesc.StructureByteStride = 0;

	HRESULT result = _direct3D->GetDevice()->CreateBuffer(&textureBufferDesc, nullptr, &_buffer);
	if (FAILED(result)) throw Exception("Failed to create the buffer for the texture description");
}

void TextureBuffer::SetShaderParameters(ShaderParameters parameters)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map texture buffer to the Device Context.");

	Buffer* textureData = static_cast<Buffer*>(mappedResource.pData);

	textureData->texturesIncluded = parameters.TextureCount;

	if (parameters.LightMapEnabled)
		textureData->lightMapEnabled = 1.0f;
	else
		textureData->lightMapEnabled = 0.0f;

	_direct3D->GetDeviceContext()->Unmap(_buffer, 0);
	_direct3D->GetDeviceContext()->PSSetConstantBuffers(parameters.BufferIndex, 1, &_buffer);
}