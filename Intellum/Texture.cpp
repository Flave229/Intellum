#include "Texture.h"

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename): _device(device), _deviceContext(deviceContext), _targaData(nullptr), _texture(nullptr), _textureView(nullptr)
{
	Initialise(filename);
}

Texture::Texture(const Texture& other) : _device(other._device), _deviceContext(other._deviceContext), _targaData(other._targaData), _texture(other._texture), _textureView(other._textureView)
{
}

Texture::~Texture()
{
}

void Texture::Initialise(char* filename)
{
	int height;
	int width;
	bool result = LoadTarga(filename, height, width);
	if (!result)
	{
		throw Exception("Failed to load Targa file: '" + string(filename) + "'");
	}

	D3D11_TEXTURE2D_DESC textureDescription = SetupAndReturnD3D11TextureDescription(height, width);

	unsigned int rowPitch = (width * 4) * sizeof(unsigned char);

	_deviceContext->UpdateSubresource(_texture, 0, nullptr, _targaData, rowPitch, 0);

	SetupD3D11ShaderResourceViewDescription(textureDescription);

	_deviceContext->GenerateMips(_textureView);

	delete _targaData;
	_targaData = nullptr;
}

D3D11_TEXTURE2D_DESC Texture::SetupAndReturnD3D11TextureDescription(int height, int width)
{
	D3D11_TEXTURE2D_DESC textureDescription;
	textureDescription.Height = height;
	textureDescription.Width = width;
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

	if(_targaData)
	{
		delete[] _targaData;
		_targaData = nullptr;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return _textureView;
}

bool Texture::LoadTarga(char* filename, int& height, int& width)
{
	int error;
	int bpp;
	int imageSize;
	int index;
	int i;
	int j;
	int k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	// Open the file ready for reading
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) return false;

	// Read file header information
	count = static_cast<unsigned int>(fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr));
	if (count != 1) return false;

	// Extract necessary data from the file header
	height = static_cast<int>(targaFileHeader.height);
	width = static_cast<int>(targaFileHeader.width);
	bpp = static_cast<int>(targaFileHeader.bpp);

	// Check that file is 32 bit
	if (bpp != 32) return false;

	imageSize = width * height * 4;

	// Allocate memory
	targaImage = new unsigned char[imageSize];
	if (!targaImage) return false;

	count = static_cast<unsigned int>(fread(targaImage, 1, imageSize, filePtr));
	if (count != imageSize) return false;

	error = fclose(filePtr);
	if (error != 0) return false;

	// Allocate memory for targa data
	_targaData = new unsigned char[imageSize];
	if(!_targaData) return false;

	index = 0;
	k = (width * height * 4) - (width * 4);

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			_targaData[index] = targaImage[k + 2];
			_targaData[index + 1] = targaImage[k + 1];
			_targaData[index + 2] = targaImage[k];
			_targaData[index + 3] = targaImage[k + 3];

			k += 4;
			index += 4;
		}

		k -= (width * 8);
	}

	delete[] targaImage;
	targaImage = nullptr;

	return true;
}