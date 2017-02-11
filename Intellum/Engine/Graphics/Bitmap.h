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
	
public:
	Bitmap(DirectX3D* direct3D, IShaderType* shader, IAppearance* appearance);
	~Bitmap();

	void Shutdown();

	void Update(XMFLOAT2 position, Box bitmapSize) const;
	void Render() const;
};