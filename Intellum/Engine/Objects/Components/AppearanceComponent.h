#pragma once
#include "../Geometry/Geometry.h"
#include "IComponent.h"
#include "../../ShaderEngine/ShaderController.h"
#include "../Texture/Texture.h"

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
	bool RenderEnabled;

	AppearanceComponent()
		: IComponent(APPEARANCE), ShaderType(SHADER_DEFAULT), Model(Geometry()), Textures(vector<Texture*>()), LightMap(nullptr), BumpMap(nullptr), Color(ColorShaderParameters()), Gradient(GradientShaderParameters()), RenderEnabled(true)
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
	}
};
