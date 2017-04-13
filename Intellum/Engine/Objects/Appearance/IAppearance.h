#pragma once
#include "../../ShaderEngine/FontShader.h"
#include "Texture/Texture.h"
#include "Texture/CreateTexture.h"

class IAppearance
{
public:
	IAppearance() {}
	virtual ~IAppearance() {}

	virtual void Shutdown() = 0;
	virtual void Render() const = 0;

	virtual void Update(XMFLOAT2 position, Box bitmapSize) = 0;

	virtual int GetIndexCount() const = 0;
	virtual Texture* GetTexture() const = 0;
	virtual vector<ID3D11ShaderResourceView*> GetTextures() const = 0;
	virtual ID3D11ShaderResourceView* GetLightMap() const = 0;
};