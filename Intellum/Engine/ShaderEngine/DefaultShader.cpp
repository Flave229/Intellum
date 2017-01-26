#include "DefaultShader.h"

DefaultShader::DefaultShader(DirectX3D* direct3D, Camera* camera, Light* light) : IShaderType(direct3D, camera, light)
{
}

DefaultShader::~DefaultShader()
{
}

void DefaultShader::Initialise(HWND hwnd)
{
	InitialiseShader(hwnd, L"shaders/VertexShader.hlsl", L"shaders/PixelShader.hlsl");
}

void DefaultShader::InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	try
	{
		ID3D10Blob* errorMessage = nullptr;

		ID3D10Blob* vertexShaderBuffer = nullptr;
		HRESULT result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "DefaultVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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

			throw Exception("Vertex Shader file missing");
		}

		ID3D10Blob* pixelShaderBuffer = nullptr;
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

			throw Exception("Pixel Shader file missing");
		}

		// Create Vertex Shader from the buffer
		result = _direct3D->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &_vertexShader);
		if (FAILED(result)) throw Exception("Failed to create the vertex shader");

		// Create Pixel Shader from the buffer
		result = _direct3D->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &_pixelShader);
		if (FAILED(result)) throw Exception("Failed to create the pixel shader");

		// Vertex Shader layout description
		// The setup below NEEDS to match the VertexType structure defined in the shader file, otherwise the bytes of data become missalligned / errors occur
		D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
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

		unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		result = _direct3D->GetDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
		if (FAILED(result)) throw Exception("Failed to create the input layout");

		vertexShaderBuffer->Release();
		vertexShaderBuffer = nullptr;

		pixelShaderBuffer->Release();
		pixelShaderBuffer = nullptr;

		// Constant Buffer description
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(ConstantBuffer);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		result = _direct3D->GetDevice()->CreateBuffer(&matrixBufferDesc, nullptr, &_matrixBuffer);
		if (FAILED(result)) throw Exception("Failed to create the buffer for the matrix description");

		// Camera Buffer Description
		D3D11_BUFFER_DESC cameraBufferDesc;
		cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cameraBufferDesc.ByteWidth = sizeof(CameraBuffer);
		cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cameraBufferDesc.MiscFlags = 0;
		cameraBufferDesc.StructureByteStride = 0;

		result = _direct3D->GetDevice()->CreateBuffer(&cameraBufferDesc, nullptr, &_cameraBuffer);
		if (FAILED(result)) throw Exception("Failed to create the buffer for the camera description");

		// Light Buffer Description
		D3D11_BUFFER_DESC lightBufferDesc;
		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(LightBuffer);
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;

		result = _direct3D->GetDevice()->CreateBuffer(&lightBufferDesc, nullptr, &_lightBuffer);
		if (FAILED(result)) throw Exception("Failed to create the buffer for the light description");

		// Sampler State Description
		D3D11_SAMPLER_DESC samplerDesc;
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
		if (FAILED(result)) throw Exception("Failed to create the sampler state");
	}
	catch(Exception& exception)
	{
		throw Exception("And error occured initialising the Default Shader");
	}
	catch(...)
	{
		throw Exception("And error occured initialising the Default Shader");
	}
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

void DefaultShader::Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	SetShaderParameters(worldMatrix, projectionMatrix, texture);

	RenderShader(indexCount);
}

void DefaultShader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	try
	{
		XMMATRIX viewMatrix;
		_camera->MapViewMatrixInto(viewMatrix);

		SetMatrixBuffer(worldMatrix, projectionMatrix, viewMatrix, 0);
		SetCameraBuffer(1);
		SetLightBuffer(0);

		_direct3D->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	}
	catch(Exception& exception)
	{
		throw Exception("Error when setting Shader Parameters in Default Shader: ", exception);
	}
	catch (...)
	{
		throw Exception("Error when setting Shader Parameters in Default Shader: ");
	}
}

void DefaultShader::SetMatrixBuffer(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX viewMatrix, unsigned int bufferNumber) const
{
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map matrix buffer to the Device Context.");

	ConstantBuffer* matrixDataPtr = static_cast<ConstantBuffer*>(mappedResource.pData);
	matrixDataPtr->world = worldMatrix;
	matrixDataPtr->view = viewMatrix;
	matrixDataPtr->projection = projectionMatrix;

	_direct3D->GetDeviceContext()->Unmap(_matrixBuffer, 0);
	_direct3D->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);
}

void DefaultShader::SetCameraBuffer(unsigned int bufferNumber) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map camera buffer to the Device Context.");

	CameraBuffer* cameraDataPtr = static_cast<CameraBuffer*>(mappedResource.pData);
	cameraDataPtr->cameraPosition = _camera->GetPosition();
	cameraDataPtr->padding = 0.0f;

	_direct3D->GetDeviceContext()->Unmap(_cameraBuffer, 0);
	_direct3D->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &_cameraBuffer);
}

void DefaultShader::SetLightBuffer(unsigned bufferNumber) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map light buffer to the Device Context.");

	LightBuffer* lightDataPtr = static_cast<LightBuffer*>(mappedResource.pData);
	lightDataPtr->ambientColor = _light->GetAmbientColor();
	lightDataPtr->diffuseColor = _light->GetDiffuseColor();
	lightDataPtr->lightDirection = _light->GetDirection();
	lightDataPtr->specularColor = _light->GetSpecularColor();
	lightDataPtr->specularPower = _light->GetSpecularPower();

	_direct3D->GetDeviceContext()->Unmap(_lightBuffer, 0);

	bufferNumber = 0;

	_direct3D->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &_lightBuffer);
}

void DefaultShader::RenderShader(int indexCount)
{
	_direct3D->GetDeviceContext()->IASetInputLayout(_layout);

	_direct3D->GetDeviceContext()->VSSetShader(_vertexShader, nullptr, 0);
	_direct3D->GetDeviceContext()->PSSetShader(_pixelShader, nullptr, 0);

	_direct3D->GetDeviceContext()->PSSetSamplers(0, 1, &_sampleState);
	_direct3D->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
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