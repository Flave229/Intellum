#pragma once
#include "../Geometry/Geometry.h"
#include "../Appearance/Texture/Texture.h"
#include "IComponent.h"
#include "../../ShaderEngine/ShaderController.h"
#include "../Appearance/ColorShaderParameters.h"

class AppearanceComponent : public IComponent
{
public:
	ShaderType ShaderType;

	Geometry Model;
	vector<Texture*> Textures;
	Texture* LightMap;
	Texture* BumpMap;
	ColorShaderParameters Color;
	GradientShaderParameters Gradient;

	AppearanceComponent()
		: IComponent(APPEARANCE), ShaderType(SHADER_DEFAULT), Model(Geometry()), Textures(vector<Texture*>()), LightMap(nullptr), BumpMap(nullptr), Color(ColorShaderParameters()), Gradient(GradientShaderParameters())
	{
	}
	
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
