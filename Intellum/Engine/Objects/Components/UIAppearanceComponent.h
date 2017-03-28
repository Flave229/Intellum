#pragma once
#include "IComponent.h"
#include "../Appearance/Texture/Texture.h"
#include "../Geometry/Geometry.h"
#include "../../ShaderEngine/ShaderController.h"

class UIAppearanceComponent : public IComponent
{
public:
	ShaderType ShaderType;

	Geometry Model;
	vector<Texture*> Textures;
	Texture* LightMap;
	Texture* BumpMap;

	UIAppearanceComponent()
		: IComponent(UI_APPEARANCE), ShaderType(SHADER_UI), Model(Geometry()), Textures(vector<Texture*>()), LightMap(nullptr), BumpMap(nullptr) { }

	~UIAppearanceComponent() override = default;

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