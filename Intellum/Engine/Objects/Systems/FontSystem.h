#pragma once
#include "ISystem.h"
#include "../../DirectX3D.h"
#include "../../ShaderEngine/ShaderController.h"
#include "../Components/TextComponent.h"
#include "../../FontEngine/FontEngine.h"

class FontSystem : public ISystem
{
private:
	DirectX3D* _direct3D;
	ShaderController* _shaderController;
	FontEngine* _fontEngine;
	HWND _hwnd;
	Box _screenSize;
	void RenderCharacters(vector<Entity*> entities); 
	void BuildBufferInformation(Entity* entity, AppearanceComponent* appearance) const; 
	ShaderResources BuildShaderResources(AppearanceComponent* appearance, TransformComponent* transform) const;
	static vector<ID3D11ShaderResourceView*> FontSystem::ExtractResourceViewsFrom(vector<Texture*> textures);

public:
	FontSystem(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, HWND hwnd, Box screenSize);
	~FontSystem();
	void Shutdown() override;

	void Update(vector<Entity*> entities, float delta) override;
	void Render(vector<Entity*> entities) override;
};
