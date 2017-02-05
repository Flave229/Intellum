#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <stdio.h>

#include "../../ErrorHandling/Exception.h"
#include "../../Loaders/TargaLoader.h"
#include "../../Common/Box.h"

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
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	ID3D11Texture2D* _texture;
	ID3D11ShaderResourceView* _textureView[2];
	int _textureCount;

private:
	void Initialise(vector<char*> filenames);

	D3D11_TEXTURE2D_DESC SetupAndReturnDX11TextureDescription(Box textureBox);
	static D3D11_SHADER_RESOURCE_VIEW_DESC SetupDX11ShaderResourceViewDescription(D3D11_TEXTURE2D_DESC textureDescription);

public:
	Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<char*> filenames);
	~Texture();

	void Shutdown();

	ID3D11ShaderResourceView** GetTextures();
	int GetTextureCount() const;
};

#endif