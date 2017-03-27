#pragma once
#include "../Geometry/Geometry.h"
#include "../Appearance/Texture/Texture.h"
#include "IComponent.h"
#include "../../ShaderEngine/IShaderType.h"

class AppearanceComponent : public IComponent
{
public:
	IShaderType* Shader;

	Geometry Model;
	vector<Texture*> Textures;
	Texture* LightMap;
	Texture* BumpMap;

	AppearanceComponent() 
		: IComponent(APPEARANCE), Shader(nullptr), Model(Geometry()), Textures(vector<Texture*>()), LightMap(nullptr), BumpMap(nullptr) {}

	~AppearanceComponent() override = default;

	void Shutdown() override 
	{
		if (LightMap)
		{
			LightMap->Shutdown();
			delete LightMap;
			LightMap = nullptr;
		}

		if (BumpMap)
		{
			BumpMap->Shutdown();
			delete BumpMap;
			BumpMap = nullptr;
		}

		for (unsigned long long i = Textures.size(); i > 0; i--)
		{
			Textures.back()->Shutdown();
			Textures.pop_back();
		}

		Textures.clear();
	}
};
