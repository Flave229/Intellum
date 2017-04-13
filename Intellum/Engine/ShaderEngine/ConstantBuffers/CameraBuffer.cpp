#include "CameraBuffer.h"

CameraBuffer::CameraBuffer(DirectX3D* direct3D, Camera* camera) : IShaderBuffer(direct3D), _buffer(nullptr), _camera(camera)
{
	CameraBuffer::Initialise();
}

void CameraBuffer::Shutdown()
{
	if (_buffer)
	{
		_buffer->Release();
		_buffer = nullptr;
	}
}

void CameraBuffer::Initialise()
{
	D3D11_BUFFER_DESC cameraBufferDesc;
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(Buffer);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	HRESULT result = _direct3D->GetDevice()->CreateBuffer(&cameraBufferDesc, nullptr, &_buffer);
	if (FAILED(result)) throw Exception("Failed to create the buffer for the camera description");
}

void CameraBuffer::SetShaderParameters(ShaderParameters parameters)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map camera buffer to the Device Context.");

	Buffer* cameraDataPtr = static_cast<Buffer*>(mappedResource.pData);
	cameraDataPtr->cameraPosition = _camera->GetTransform()->GetPosition();
	cameraDataPtr->padding = 0.0f;

	_direct3D->GetDeviceContext()->Unmap(_buffer, 0);
	_direct3D->GetDeviceContext()->VSSetConstantBuffers(parameters.BufferIndex, 1, &_buffer);
}

void CameraBuffer::SetShaderParameters(int bufferIndex, ShaderResources shaderResources)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map camera buffer to the Device Context.");

	Buffer* cameraDataPtr = static_cast<Buffer*>(mappedResource.pData);
	cameraDataPtr->cameraPosition = _camera->GetTransform()->GetPosition();
	cameraDataPtr->padding = 0.0f;

	_direct3D->GetDeviceContext()->Unmap(_buffer, 0);
	_direct3D->GetDeviceContext()->VSSetConstantBuffers(bufferIndex, 1, &_buffer);
}