#pragma once
#include "../../../common/Geometry.h"
#include "../Appearance/Texture/Texture.h"
#include "IComponent.h"

class AppearanceComponent : public IComponent
{
public:
	Geometry Model;
	vector<Texture*> Textures;
	Texture* LightMap;
	Texture* BumpMap;

	AppearanceComponent() 
		: AppearanceComponent(Geometry(), vector<Texture*>(), nullptr) {}

	AppearanceComponent(Geometry model, vector<Texture*> textures, Texture* lightMap) 
		: IComponent(APPEARANCE), Model(model), Textures(textures), LightMap(lightMap), BumpMap(nullptr) {}

	~AppearanceComponent() override = default;
};