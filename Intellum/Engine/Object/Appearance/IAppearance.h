#pragma once
#include "../../ShaderEngine/FontShader.h"
#include "../../Graphics/Texture.h"

class IAppearance
{
public:
	IAppearance() {}
	virtual ~IAppearance() {}

	virtual void Shutdown() = 0;
	virtual void Render() const = 0;

	virtual void Update(XMFLOAT2 position, Box bitmapSize) = 0;

	virtual void LoadTextures(vector<char*> filenames) = 0;

	virtual int GetIndexCount() const = 0;
	virtual int GetTextureCount() const = 0;
	virtual vector<ID3D11ShaderResourceView*> GetTextures() const = 0;
};