#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../../Engine/ShaderEngine/IShaderType.h"
#include "../../Engine/ShaderEngine/DefaultShader.h"
#include "../../Engine/DirectX3D.h"
#include "../../common/Vertex.h"
#include "Texture.h"
#include "Light.h"
#include "../Object/Appearance/IAppearance.h"

using namespace DirectX;

class Bitmap
{
private:
	DirectX3D* _direct3D;
	IShaderType* _shader;
	IAppearance* _appearance;

	Box* _screenSize;
	Box* _bitmapSize;
	XMFLOAT2* _previousPosition;
	
private:
	void UpdateBuffers(XMFLOAT2 position, Box bitmapSize);
public:
	Bitmap(DirectX3D* direct3D, IShaderType* shader, IAppearance* appearance, Box* screenSize, Box* bitmapBox, XMFLOAT2* position);
	~Bitmap();

	void Shutdown();

	void Update(XMFLOAT2 position, Box bitmapSize);
	void Render() const;

	IAppearance* GetAppearance() const;
};