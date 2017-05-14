#include "LightBuffer.h"

LightBuffer::LightBuffer(DirectX3D* direct3D, Light* light) : IShaderBuffer(direct3D), _buffer(nullptr), _light(light)
{
	LightBuffer::Initialise();
}

void LightBuffer::Shutdown()
{
	if (_buffer)
	{
		_buffer->Release();
		_buffer = nullptr;
	}
}

void LightBuffer::Initialise()
{
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(Buffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	HRESULT result = _direct3D->GetDevice()->CreateBuffer(&lightBufferDesc, nullptr, &_buffer);
	if (FAILED(result)) throw Exception("Failed to create the buffer for the light description");
}

void LightBuffer::SetShaderParameters(int bufferIndex, ShaderResources shaderResources)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map light buffer to the Device Context.");

	Buffer* lightDataPtr = static_cast<Buffer*>(mappedResource.pData);
	lightDataPtr->lightEnabled = shaderResources.LightEnabled;
	if (shaderResources.LightEnabled)
	{
		lightDataPtr->ambientColor = _light->AmbientColor;
		lightDataPtr->diffuseColor = _light->DiffuseColor;
		lightDataPtr->lightDirection = _light->GetDirection(shaderResources.MatrixParameters.WorldMatrix);
		lightDataPtr->specularColor = _light->SpecularColor;
		lightDataPtr->specularPower = _light->SpecularPower;
	}

	_direct3D->GetDeviceContext()->Unmap(_buffer, 0);
	_direct3D->GetDeviceContext()->PSSetConstantBuffers(bufferIndex, 1, &_buffer);
}