#include "UIShader.h"

UIShader::UIShader(DirectX3D* direct3D, Camera* camera, Light* light) : IShaderType(direct3D, camera, light), _colorOverloadEnabled(false)
{
}

UIShader::~UIShader()
{
}

void UIShader::Initialise(HWND hwnd)
{
	InitialiseShader(hwnd, L"shaders/FontVertexShader.hlsl", L"shaders/FontPixelShader.hlsl");
}

void UIShader::InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	try
	{
		ID3D10Blob* errorMessage = nullptr;

		ID3D10Blob* vertexShaderBuffer = nullptr;
		HRESULT result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "FontVertexShader", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer, &errorMessage);
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
		result = D3DCompileFromFile(psFilename, nullptr, nullptr, "FontPixelShader", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);

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

		_matrixBuffer = new MatrixBuffer(_direct3D);
		_cameraBuffer = new CameraBuffer(_direct3D, _camera);
		_colorBuffer = new ColorOverrideBuffer(_direct3D);
		_textureBuffer = new TextureBuffer(_direct3D);

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
		throw Exception("And error occured initialising the Font Shader", exception);
	}
	catch (...)
	{
		throw Exception("And error occured initialising the Font Shader");
	}
}

void UIShader::Shutdown()
{
	if (_matrixBuffer)
	{
		_matrixBuffer->Shutdown();
		_matrixBuffer = nullptr;
	}

	if (_cameraBuffer)
	{
		_cameraBuffer->Shutdown();
		_cameraBuffer = nullptr;
	}

	if (_colorBuffer)
	{
		_colorBuffer->Shutdown();
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

void UIShader::Render(int indexCount, ShaderResources shaderResources, XMMATRIX worldMatrix, XMMATRIX projectionMatrix)
{
	SetShaderParameters(shaderResources, worldMatrix, projectionMatrix);

	RenderShader(indexCount);
}

void UIShader::SetShaderParameters(ShaderResources shaderResources, XMMATRIX worldMatrix, XMMATRIX projectionMatrix)
{
	try
	{
		_matrixBuffer->SetShaderParameters(ShaderParameterConstructor::ConstructMatrixBufferParameters(0, worldMatrix, projectionMatrix, _viewMatrix));
		_cameraBuffer->SetShaderParameters(ShaderParameterConstructor::ConstructDefaultBufferParameters(1));
		_colorBuffer->SetShaderParameters(ShaderParameterConstructor::ConstructColorOverloadBufferParameters(2, shaderResources.ColorParameters));

		bool lightMapEnabled = false;
		if (shaderResources.TextureParameters.LightMap != nullptr)
			lightMapEnabled = true;

		bool bumpMapEnabled = false;
		if (shaderResources.TextureParameters.BumpMap != nullptr)
			bumpMapEnabled = true;

		int textureCount = static_cast<int>(shaderResources.TextureParameters.TextureArray.size());
		_textureBuffer->SetShaderParameters(ShaderParameterConstructor::ConstructTextureBufferParameters(3, textureCount, lightMapEnabled, bumpMapEnabled));

		_direct3D->GetDeviceContext()->PSSetShaderResources(0, textureCount, &shaderResources.TextureParameters.TextureArray.at(0));

		if (lightMapEnabled)
			_direct3D->GetDeviceContext()->PSSetShaderResources(10, 1, &shaderResources.TextureParameters.LightMap);
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

void UIShader::RenderShader(int indexCount)
{
	_direct3D->GetDeviceContext()->IASetInputLayout(_layout);

	_direct3D->GetDeviceContext()->VSSetShader(_vertexShader, nullptr, 0);
	_direct3D->GetDeviceContext()->PSSetShader(_pixelShader, nullptr, 0);

	_direct3D->GetDeviceContext()->PSSetSamplers(0, 1, &_sampleState);
	_direct3D->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void UIShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName)
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

void UIShader::SetColorOverload(bool state, XMFLOAT4 color)
{
	_colorOverloadEnabled = state;
	_colorOverload = color;
}