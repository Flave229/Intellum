#pragma once
#include "../Geometry/Geometry.h"
#include "../Appearance/Texture/Texture.h"
#include "IComponent.h"
#include "../../ShaderEngine/IShaderType.h"

class AppearanceComponent : public IComponent
{
public:
	Geometry Model;
	IShaderType* Shader;
	vector<Texture*> Textures;
	Texture* LightMap;
	Texture* BumpMap;

	AppearanceComponent() 
		: AppearanceComponent(Geometry(), vector<Texture*>(), nullptr) {}

	AppearanceComponent(Geometry model, vector<Texture*> textures, Texture* lightMap) 
		: IComponent(APPEARANCE), Model(model), Textures(textures), LightMap(lightMap), BumpMap(nullptr) {}

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
