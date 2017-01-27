#include "FontShader.h"

FontShader::FontShader(DirectX3D* direct3D, Camera* camera, Light* light) : IShaderType(direct3D, camera, light)
{
}

FontShader::~FontShader()
{
}

void FontShader::Initialise(HWND hwnd)
{
	InitialiseShader(hwnd, L"shaders/FontVertexShader.hlsl", L"shaders/FontPixelShader.hlsl");
}

void FontShader::InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	try
	{
		ID3D10Blob* errorMessage = nullptr;

		ID3D10Blob* vertexShaderBuffer = nullptr;
		HRESULT result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "FontVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
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

		// Camera Buffer Description
		D3D11_BUFFER_DESC colorBufferDesc;
		colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		colorBufferDesc.ByteWidth = sizeof(ColorBuffer);
		colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		colorBufferDesc.MiscFlags = 0;
		colorBufferDesc.StructureByteStride = 0;

		result = _direct3D->GetDevice()->CreateBuffer(&colorBufferDesc, nullptr, &_colorBuffer);
		if (FAILED(result)) throw Exception("Failed to create the buffer for the color description");

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
		
		_viewMatrix = _camera->GetViewMatrix();
	}
	catch (Exception& exception)
	{
		throw Exception("And error occured initialising the Font Shader");
	}
	catch (...)
	{
		throw Exception("And error occured initialising the Font Shader");
	}
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

void FontShader::Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	SetShaderParameters(worldMatrix, projectionMatrix, texture);

	RenderShader(indexCount);
}

void FontShader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	try
	{
		SetMatrixBuffer(worldMatrix, projectionMatrix, _viewMatrix, 0);
		SetCameraBuffer(1); 
		SetColorBuffer(0);

		_direct3D->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	}
	catch (Exception& exception)
	{
		throw Exception("Error when setting Shader Parameters in Font Shader: ", exception);
	}
	catch (...)
	{
		throw Exception("Error when setting Shader Parameters in Font Shader: ");
	}
}

void FontShader::SetMatrixBuffer(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX viewMatrix, unsigned int bufferNumber) const
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

void FontShader::SetCameraBuffer(unsigned bufferNumber) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map camera buffer to the Device Context.");

	CameraBuffer* cameraDataPtr = static_cast<CameraBuffer*>(mappedResource.pData);
	cameraDataPtr->cameraPosition = _camera->GetTransform()->GetPosition();
	cameraDataPtr->padding = 0.0f;

	_direct3D->GetDeviceContext()->Unmap(_cameraBuffer, 0);
	_direct3D->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &_cameraBuffer);
}

void FontShader::SetColorBuffer(unsigned bufferNumber) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map color buffer to the Device Context.");

	ColorBuffer* colorDataPtr = static_cast<ColorBuffer*>(mappedResource.pData);

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
	_direct3D->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &_colorBuffer);
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