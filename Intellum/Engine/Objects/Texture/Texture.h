#pragma once

#include <d3d11.h>
#include <stdio.h>

#include "../../../ErrorHandling/Exception.h"
#include "../../../Loaders/TargaLoader.h"
#include "../../../common/Box.h"
#include "../../DirectX3D.h"

using namespace std;

class Texture
{
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

private:
	ID3D11Texture2D* _texture;
	ID3D11ShaderResourceView* _textureView;

private:
	void Initialise(DirectX3D* direct3d, char* filename);

	D3D11_TEXTURE2D_DESC SetupAndReturnDX11TextureDescription(ID3D11Device* device, Box textureBox);
	static D3D11_SHADER_RESOURCE_VIEW_DESC SetupDX11ShaderResourceViewDescription(D3D11_TEXTURE2D_DESC textureDescription);

public:
	Texture(DirectX3D* direct3d, char* filename);
	~Texture();

	void Shutdown();

	ID3D11ShaderResourceView* GetTexture() const;
};