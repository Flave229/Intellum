#include "FontShader.h"

FontShader::FontShader(DirectX3D* direct3D, Camera* camera) : IShaderType(direct3D, camera)
{
}

FontShader::~FontShader()
{
}

bool FontShader::Initialise(HWND hwnd)
{
	bool result = InitialiseShader(hwnd, L"shaders/FontVertexShader.hlsl", L"shaders/FontPixelShader.hlsl");
	if (!result) return false;

	return true;
}

bool FontShader::InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC colorBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "FontVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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

	result = D3DCompileFromFile(psFilename, nullptr, nullptr, "FontPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

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
	result = _direct3D->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &_vertexShader);
	if (FAILED(result)) return false;

	// Create Pixel Shader from the buffer
	result = _direct3D->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &_pixelShader);
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

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = _direct3D->GetDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
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

	result = _direct3D->GetDevice()->CreateBuffer(&matrixBufferDesc, nullptr, &_matrixBuffer);
	if (FAILED(result)) return false;

	// Camera Buffer Description
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBuffer);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	result = _direct3D->GetDevice()->CreateBuffer(&cameraBufferDesc, nullptr, &_cameraBuffer);
	if (FAILED(result)) return false;

	// Camera Buffer Description
	colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	colorBufferDesc.ByteWidth = sizeof(ColorBuffer);
	colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colorBufferDesc.MiscFlags = 0;
	colorBufferDesc.StructureByteStride = 0;

	result = _direct3D->GetDevice()->CreateBuffer(&colorBufferDesc, nullptr, &_colorBuffer);
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

	result = _direct3D->GetDevice()->CreateSamplerState(&samplerDesc, &_sampleState);
	if (FAILED(result)) return false;

	return true;
}

void FontShader::Shutdown()
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

	if (_colorBuffer)
	{
		_colorBuffer->Release();
		_colorBuffer = nullptr;
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

bool FontShader::Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, Light* light)
{
	bool result = SetShaderParameters(worldMatrix, projectionMatrix, texture, light);
	if (!result) return false;

	RenderShader(indexCount);

	return true;
}

bool FontShader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, Light* light)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstantBuffer* matrixDataPtr;
	CameraBuffer* cameraDataPtr;
	ColorBuffer* colorDataPtr;
	unsigned int bufferNumber;

	XMMATRIX viewMatrix;
	_camera->MapViewMatrixInto(viewMatrix);

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	result = _direct3D->GetDeviceContext()->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	matrixDataPtr = static_cast<ConstantBuffer*>(mappedResource.pData);

	matrixDataPtr->world = worldMatrix;
	matrixDataPtr->view = viewMatrix;
	matrixDataPtr->projection = projectionMatrix;

	_direct3D->GetDeviceContext()->Unmap(_matrixBuffer, 0);

	bufferNumber = 0;

	_direct3D->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	result = _direct3D->GetDeviceContext()->Map(_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	cameraDataPtr = static_cast<CameraBuffer*>(mappedResource.pData);

	cameraDataPtr->cameraPosition = _camera->GetPosition();
	cameraDataPtr->padding = 0.0f;

	_direct3D->GetDeviceContext()->Unmap(_cameraBuffer, 0);

	bufferNumber = 1;

	_direct3D->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &_cameraBuffer);

	_direct3D->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	
	result = _direct3D->GetDeviceContext()->Map(_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	colorDataPtr = static_cast<ColorBuffer*>(mappedResource.pData);

	if (_colorOverloadEnabled)
	{
		colorDataPtr->colorOverloadEnabled = 1.0f;
		colorDataPtr->colorOverload = _fontColor;
	}
	else
	{
		colorDataPtr->colorOverloadEnabled = 0.0f;
		colorDataPtr->colorOverload = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	colorDataPtr->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	_direct3D->GetDeviceContext()->Unmap(_colorBuffer, 0);

	bufferNumber = 0;

	_direct3D->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &_colorBuffer);

	return true;
}

void FontShader::RenderShader(int indexCount)
{
	_direct3D->GetDeviceContext()->IASetInputLayout(_layout);

	_direct3D->GetDeviceContext()->VSSetShader(_vertexShader, nullptr, 0);
	_direct3D->GetDeviceContext()->PSSetShader(_pixelShader, nullptr, 0);

	_direct3D->GetDeviceContext()->PSSetSamplers(0, 1, &_sampleState);
	_direct3D->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}


void FontShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName)
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

void FontShader::SetColorOverload(bool state, XMFLOAT4 color)
{
	_colorOverloadEnabled = state;
	_fontColor = color;
}