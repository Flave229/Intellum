#include "Texture.h"

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename): _device(device), _deviceContext(deviceContext), _texture(nullptr), _textureView(nullptr)
{
	Initialise(filename);
}

Texture::Texture(const Texture& other) : _device(other._device), _deviceContext(other._deviceContext), _texture(other._texture), _textureView(other._textureView)
{
}

Texture::~Texture()
{
}

void Texture::Initialise(char* filename)
{
	TargaData targaData = TargaLoader::LoadTarga(filename);

	D3D11_TEXTURE2D_DESC textureDescription = SetupAndReturnD3D11TextureDescription(targaData.ImageSize);

	unsigned int rowPitch = (targaData.ImageSize.Width * 4) * sizeof(unsigned char);

	_deviceContext->UpdateSubresource(_texture, 0, nullptr, targaData.ImageData, rowPitch, 0);

	SetupD3D11ShaderResourceViewDescription(textureDescription);

	_deviceContext->GenerateMips(_textureView);

	delete[] targaData.ImageData;
	targaData.ImageData = nullptr;
}

D3D11_TEXTURE2D_DESC Texture::SetupAndReturnD3D11TextureDescription(Box textureBox)
{
	D3D11_TEXTURE2D_DESC textureDescription;
	textureDescription.Height = static_cast<UINT>(textureBox.Height);
	textureDescription.Width = static_cast<UINT>(textureBox.Width);
	textureDescription.MipLevels = 0;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	HRESULT hResult = _device->CreateTexture2D(&textureDescription, nullptr, &_texture);

	if (FAILED(hResult))
		throw Exception("Failed to create texture description");

	return textureDescription;
}

void Texture::SetupD3D11ShaderResourceViewDescription(D3D11_TEXTURE2D_DESC textureDescription)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDescription.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	HRESULT hResult = _device->CreateShaderResourceView(_texture, &srvDesc, &_textureView);

	if (FAILED(hResult))
		throw Exception("Failed to create the Shader Resource View");
}

void Texture::Shutdown()
{
	if(_textureView)
	{
		_textureView->Release();
		_textureView = nullptr;
	}

	if(_texture)
	{
		_texture->Release();
		_texture = nullptr;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return _textureView;
}