#include "GradientOverloadBuffer.h"

GradientOverloadBuffer::GradientOverloadBuffer(DirectX3D* direct3D) : IShaderBuffer(direct3D), _buffer(nullptr)
{
	GradientOverloadBuffer::Initialise();
}

void GradientOverloadBuffer::Shutdown()
{
	if (_buffer)
	{
		_buffer->Release();
		_buffer = nullptr;
	}
}

void GradientOverloadBuffer::Initialise()
{
	D3D11_BUFFER_DESC gradientBufferDesc;
	gradientBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	gradientBufferDesc.ByteWidth = sizeof(Buffer);
	gradientBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gradientBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gradientBufferDesc.MiscFlags = 0;
	gradientBufferDesc.StructureByteStride = 0;

	HRESULT result = _direct3D->GetDevice()->CreateBuffer(&gradientBufferDesc, nullptr, &_buffer);
	if (FAILED(result)) throw Exception("Failed to create the buffer for the gradient description");
}

void GradientOverloadBuffer::SetShaderParameters(ShaderParameters parameters)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map gradient buffer to the Device Context.");

	Buffer* gradientDataPointer = static_cast<Buffer*>(mappedResource.pData);

	if (parameters.GradientOverloadEnabled)
		gradientDataPointer->gradientOverloadEnabled = 1.0f;
	else
		gradientDataPointer->gradientOverloadEnabled = 0.0f;

	gradientDataPointer->apexColor = parameters.GradientApexOverload;
	gradientDataPointer->apexPosition = parameters.GradientHeight;
	gradientDataPointer->centerColor = parameters.GradientCenterOverload;
	gradientDataPointer->centerPosition = parameters.CenterPosition;

	_direct3D->GetDeviceContext()->Unmap(_buffer, 0);
	_direct3D->GetDeviceContext()->PSSetConstantBuffers(parameters.BufferIndex, 1, &_buffer);
}

void GradientOverloadBuffer::SetShaderParameters(int bufferIndex, ShaderResources shaderResources)
{
	GradientShaderParameters gradientOverload = shaderResources.GradientParameters;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map gradient buffer to the Device Context.");

	Buffer* gradientDataPointer = static_cast<Buffer*>(mappedResource.pData);

	if (gradientOverload.Enabled)
		gradientDataPointer->gradientOverloadEnabled = 1.0f;
	else
		gradientDataPointer->gradientOverloadEnabled = 0.0f;

	gradientDataPointer->apexColor = gradientOverload.ApexColor;
	gradientDataPointer->apexPosition = gradientOverload.Height;
	gradientDataPointer->centerColor = gradientOverload.CenterColor;
	gradientDataPointer->centerPosition = gradientOverload.CenterYCordinates;

	_direct3D->GetDeviceContext()->Unmap(_buffer, 0);
	_direct3D->GetDeviceContext()->PSSetConstantBuffers(bufferIndex, 1, &_buffer);
}