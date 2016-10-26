#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <stdio.h>

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
	bool LoadTarga(char*, int&, int&);
public:
	Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Texture(const Texture&);
	~Texture();

	bool Initialise(char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
};

#endif