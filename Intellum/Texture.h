#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <stdio.h>

#include "error_handling/Exception.h"

using namespace std;

class Texture
{
// Structures
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

// Member Level Variables
private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	unsigned char* _targaData;
	ID3D11Texture2D* _texture;
	ID3D11ShaderResourceView* _textureView;

// Function Declarations
private:
	void Initialise(char* filename);
	bool LoadTarga(char* filename, int& height, int& width);

	D3D11_TEXTURE2D_DESC SetupAndReturnD3D11TextureDescription(int height, int width);
	void SetupD3D11ShaderResourceViewDescription(D3D11_TEXTURE2D_DESC textureDescription);

public:
	Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	Texture(const Texture&);
	~Texture();

	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
};

#endif