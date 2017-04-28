#include "CreateTexture.h"

CreateTexture::CreateTexture()
{
}

CreateTexture::~CreateTexture()
{
}

Texture* CreateTexture::From(DirectX3D* direct3D, char* fileName)
{
	try
	{
		if (string(fileName) == "")
			return nullptr;

		return new Texture(direct3D, fileName);
	}
	catch(Exception&)
	{
		return nullptr;
	}
}

vector<Texture*> CreateTexture::ListFrom(DirectX3D* direct3D, vector<char*> fileNames)
{
	vector<Texture*> textureList;

	for (int i = 0; i < fileNames.size(); i++)
	{
		Texture* texture = From(direct3D, fileNames.at(i));

		if (texture != nullptr)
			textureList.push_back(texture);
	}

	return textureList;
}
