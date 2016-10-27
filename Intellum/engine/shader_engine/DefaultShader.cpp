#include "DefaultShader.h"

DefaultShader::DefaultShader(DirectX3D* direct3D, ID3D11Device* device, ID3D11DeviceContext* deviceContext) : IShaderType(direct3D), _device(device), _deviceContext(deviceContext)
{
}

DefaultShader::~DefaultShader()
{
}

bool DefaultShader::Initialise(HWND hwnd)
{
	bool result = InitialiseShader(hwnd, L"shaders/VertexShader.hlsl", L"shaders/PixelShader.hlsl");
	if (!result) return false;

	return true;
}

bool DefaultShader::InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "DefaultVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Vertex Shader File", MB_OK);
		}

		return false;
	}

	result = D3DCompileFromFile(psFilename, nullptr, nullptr, "DefaultPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Pixel Shader File", MB_OK);
		}

		return false;
	}

	// Create Vertex Shader from the buffer
	result = _device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &_vertexShader);
	if (FAILED(result)) return false;

	// Create Pixel Shader from the buffer
	result = _device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &_pixelShader);
	if (FAILED(result)) return false;

	// Vertex Shader layout description
	// The setup below NEEDS to match the VertexType structure defined in the shader file, otherwise the bytes of data become missalligned / errors occur
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = _device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if (FAILED(result)) return false;

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Constant Buffer description
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = _device->CreateBuffer(&matrixBufferDesc, nullptr, &_matrixBuffer);
	if (FAILED(result)) return false;

	// Camera Buffer Description
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBuffer);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	result = _device->CreateBuffer(&cameraBufferDesc, nullptr, &_cameraBuffer);
	if (FAILED(result)) return false;

	// Light Buffer Description
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = _device->CreateBuffer(&lightBufferDesc, nullptr, &_lightBuffer);
	if (FAILED(result)) return false;

	// Sampler State Description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = _device->CreateSamplerState(&samplerDesc, &_sampleState);
	if (FAILED(result)) return false;

	return true;
}

void DefaultShader::Shutdown()
{
	if (_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = nullptr;
	}

	if (_cameraBuffer)
	{
		_cameraBuffer->Release();
		_cameraBuffer = nullptr;
	}

	if (_lightBuffer)
	{
		_lightBuffer->Release();
		_lightBuffer = nullptr;
	}

	if (_layout)
	{
		_layout->Release();
		_layout = nullptr;
	}

	if (_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = nullptr;
	}

	if (_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = nullptr;
	}

	if (_sampleState)
	{
		_sampleState->Release();
		_sampleState = nullptr;
	}
}

bool DefaultShader::Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light)
{
	bool result = SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, texture, cameraPosition, light);
	if (!result) return false;

	RenderShader(indexCount);

	return true;
}

bool DefaultShader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstantBuffer* matrixDataPtr;
	CameraBuffer* cameraDataPtr;
	LightBuffer* lightDataPtr;
	unsigned int bufferNumber;

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	result = _deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	matrixDataPtr = static_cast<ConstantBuffer*>(mappedResource.pData);

	matrixDataPtr->world = worldMatrix;
	matrixDataPtr->view = viewMatrix;
	matrixDataPtr->projection = projectionMatrix;

	_deviceContext->Unmap(_matrixBuffer, 0);

	bufferNumber = 0;

	_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	result = _deviceContext->Map(_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	cameraDataPtr = static_cast<CameraBuffer*>(mappedResource.pData);

	cameraDataPtr->cameraPosition = cameraPosition;
	cameraDataPtr->padding = 0.0f;

	_deviceContext->Unmap(_cameraBuffer, 0);

	bufferNumber = 1;

	_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_cameraBuffer);

	_deviceContext->PSSetShaderResources(0, 1, &texture);

	result = _deviceContext->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	lightDataPtr = static_cast<LightBuffer*>(mappedResource.pData);

	lightDataPtr->ambientColor = light->GetAmbientColor();
	lightDataPtr->diffuseColor = light->GetDiffuseColor();
	lightDataPtr->lightDirection = light->GetDirection();
	lightDataPtr->specularColor = light->GetSpecularColor();
	lightDataPtr->specularPower = light->GetSpecularPower();

	_deviceContext->Unmap(_lightBuffer, 0);

	bufferNumber = 0;

	_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_lightBuffer);

	return true;
}

void DefaultShader::RenderShader(int indexCount)
{
	_deviceContext->IASetInputLayout(_layout);

	_deviceContext->VSSetShader(_vertexShader, nullptr, 0);
	_deviceContext->PSSetShader(_pixelShader, nullptr, 0);

	_deviceContext->PSSetSamplers(0, 1, &_sampleState);
	_deviceContext->DrawIndexed(indexCount, 0, 0);
}


void DefaultShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName)
{
	char* compileErrors;
	unsigned long long bufferSize;
	ofstream fout;

	compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error.txt");

	for (unsigned long long i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = nullptr;

	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFileName, MB_OK);
}