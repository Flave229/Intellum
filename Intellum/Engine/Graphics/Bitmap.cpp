#include "Bitmap.h"

Bitmap::Bitmap(DirectX3D* direct3D, IShaderType* shader, IAppearance* appearance) : _direct3D(direct3D), _shader(shader), _appearance(appearance)
{
}

Bitmap::~Bitmap()
{
}

void Bitmap::Shutdown()
{
	if (_appearance)
	{
		_appearance->Shutdown();
		_appearance = nullptr;
	}
}

void Bitmap::Update(XMFLOAT2 position, Box bitmapSize) const
{
	_appearance->Update(position, bitmapSize);
}

void Bitmap::Render() const
{
	_appearance->Render();

	ShaderResources shaderResources;
	shaderResources.textureArray = _appearance->GetTextures();
	shaderResources.lightMap = _appearance->GetLightMap();

	_shader->Render(_appearance->GetIndexCount(), shaderResources, _direct3D->GetWorldMatrix(), _direct3D->GetOrthoMatrix());
}