#include "Texture.h"

Texture::Texture(DirectX3D* direct3d, char* filename): _texture(nullptr), _textureView(nullptr)
{
	Initialise(direct3d, filename);
}

Texture::~Texture()
{
}

void Texture::Initialise(DirectX3D* direct3d, char* filename)
{
	ID3D11Device* device = direct3d->GetDevice();
	ID3D11DeviceContext* deviceContext = direct3d->GetDeviceContext();

	TargaData targaData = TargaLoader::LoadTarga(filename);
	D3D11_TEXTURE2D_DESC textureDescription = SetupAndReturnDX11TextureDescription(device, targaData.ImageSize);

	unsigned int rowPitch = static_cast<unsigned int>(targaData.ImageSize.Width * 4 * sizeof(unsigned char));
	deviceContext->UpdateSubresource(_texture, 0, nullptr, targaData.ImageData, rowPitch, 0);
		
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDescription = SetupDX11ShaderResourceViewDescription(textureDescription);
	HRESULT hResult = device->CreateShaderResourceView(_texture, &shaderDescription, &_textureView);

	if (FAILED(hResult))
		throw Exception("Failed to create the Shader Resource View");

	deviceContext->GenerateMips(_textureView);

	delete[] targaData.ImageData;
	targaData.ImageData = nullptr;
}

D3D11_TEXTURE2D_DESC Texture::SetupAndReturnDX11TextureDescription(ID3D11Device* device, Box textureBox)
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

	HRESULT hResult = device->CreateTexture2D(&textureDescription, nullptr, &_texture);

	if (FAILED(hResult))
		throw Exception("Failed to create texture description");

	return textureDescription;
}

D3D11_SHADER_RESOURCE_VIEW_DESC Texture::SetupDX11ShaderResourceViewDescription(D3D11_TEXTURE2D_DESC textureDescription)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDescription;
	shaderDescription.Format = textureDescription.Format;
	shaderDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDescription.Texture2D.MostDetailedMip = 0;
	shaderDescription.Texture2D.MipLevels = -1;

	return shaderDescription;
}

void Texture::Shutdown()
{
	if (_textureView)
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

ID3D11ShaderResourceView* Texture::GetTexture() const
{
	return _textureView;
}