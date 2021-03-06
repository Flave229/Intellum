#include "MatrixBuffer.h"

MatrixBuffer::MatrixBuffer(DirectX3D* direct3D) : IShaderBuffer(direct3D), _buffer(nullptr)
{
	MatrixBuffer::Initialise();
}

void MatrixBuffer::Shutdown()
{
	if (_buffer)
	{
		_buffer->Release();
		_buffer = nullptr;
	}
}

void MatrixBuffer::Initialise()
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	HRESULT result = _direct3D->GetDevice()->CreateBuffer(&matrixBufferDesc, nullptr, &_buffer);
	if (FAILED(result)) throw Exception("Failed to create the buffer for the matrix description");
}

void MatrixBuffer::SetShaderParameters(int bufferIndex, ShaderResources shaderResources)
{
	MatrixShaderParameters matrixParameters = shaderResources.MatrixParameters;
	matrixParameters.WorldMatrix = XMMatrixTranspose(matrixParameters.WorldMatrix);
	matrixParameters.ViewMatrix = XMMatrixTranspose(matrixParameters.ViewMatrix);
	matrixParameters.ProjectionMatrix = XMMatrixTranspose(matrixParameters.ProjectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map matrix buffer to the Device Context.");

	ConstantBuffer* matrixDataPtr = static_cast<ConstantBuffer*>(mappedResource.pData);
	matrixDataPtr->world = matrixParameters.WorldMatrix;
	matrixDataPtr->view = matrixParameters.ViewMatrix;
	matrixDataPtr->projection = matrixParameters.ProjectionMatrix;

	_direct3D->GetDeviceContext()->Unmap(_buffer, 0);
	_direct3D->GetDeviceContext()->VSSetConstantBuffers(bufferIndex, 1, &_buffer);
}