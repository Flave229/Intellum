#include "Bitmap.h"

Bitmap::Bitmap(DirectX3D* direct3D, IShaderType* shader, IAppearance* appearance, Box* screenSize, Box* bitmapBox, XMFLOAT2* position) : _direct3D(direct3D), _shader(shader), _appearance(appearance), _screenSize(screenSize), _bitmapSize(bitmapBox), _previousPosition(position)
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

	if (_bitmapSize)
	{
		delete _bitmapSize;
		_bitmapSize = nullptr;
	}
	
	if (_previousPosition)
	{
		delete _previousPosition;
		_previousPosition = nullptr;
	}
}

void Bitmap::Update(XMFLOAT2 position, Box bitmapSize)
{
	UpdateBuffers(position, bitmapSize);

	_appearance->Update();
}

void Bitmap::Render() const
{
	_appearance->Render();

	XMMATRIX worldMatrix = _direct3D->GetWorldMatrix();
	XMMATRIX orthoMatrix = _direct3D->GetOrthoMatrix();

	_shader->Render(_appearance->GetIndexCount(), worldMatrix, orthoMatrix, _appearance->GetTextures(), _appearance->GetTextureCount());
}

void Bitmap::UpdateBuffers(XMFLOAT2 position, Box bitmapSize)
{
	if ((position.x == static_cast<int>(_previousPosition->x)) && (position.y == static_cast<int>(_previousPosition->y)) && (bitmapSize.Width == _bitmapSize->Width) && (bitmapSize.Height == _bitmapSize->Height))
	{
		return;
	}

	memcpy(_previousPosition, &position, sizeof(XMFLOAT2));
	memcpy(_bitmapSize, &bitmapSize, sizeof(Box));
}

IAppearance* Bitmap::GetAppearance() const
{
	return _appearance;
}