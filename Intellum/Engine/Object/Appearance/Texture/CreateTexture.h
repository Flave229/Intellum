#pragma once
#include "Texture.h"
#include "../../../DirectX3D.h"
#include <string>
#include "../../../../ErrorHandling/Exception.h"
#include "../../../DirectX3D.h"

class CreateTexture
{
public:
	CreateTexture();
	~CreateTexture();

	static Texture* From(DirectX3D* direct3D, char* fileName);
	static vector<Texture*> ListFrom(DirectX3D* direct3D, vector<char*> fileName);
};
