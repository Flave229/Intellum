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
	unsigned char* _targaData;
	ID3D11Texture2D* _texture;
	ID3D11ShaderResourceView* _textureView;

// Function Declarations
private:
	bool LoadTarga(char*, int&, int&);
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
};

#endif