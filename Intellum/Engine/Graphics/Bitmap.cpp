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

	XMMATRIX worldMatrix = _direct3D->GetWorldMatrix();
	XMMATRIX orthoMatrix = _direct3D->GetOrthoMatrix();

	ID3D11ShaderResourceView* lightMap = nullptr;

	if (_appearance != nullptr)
		lightMap = _appearance->GetLightMap();

	_shader->Render(_appearance->GetIndexCount(), worldMatrix, orthoMatrix, _appearance->GetTextures(), lightMap);
}