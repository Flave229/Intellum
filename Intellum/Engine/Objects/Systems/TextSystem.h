#pragma once
#include "ISystem.h"
#include "../../DirectX3D.h"
#include "../../ShaderEngine/ShaderController.h"
#include "../Components/TextComponent.h"
#include "../../FontEngine/FontEngine.h"
#include "UISystem.h"

class TextSystem : public ISystem
{
private:
	DirectX3D* _direct3D;
	ShaderController* _shaderController;
	FontEngine* _fontEngine;
	Box _screenSize;
	XMMATRIX _viewMatrix;

	void UpdateTextEntity(TextComponent* textComponent);
	void UpdateAppearance(TextTexture& texture);
	void RenderCharacters(vector<TextTexture>& entities);
	void BuildBufferInformation(TextTexture& character) const;
	ShaderResources BuildShaderResources(TextTexture& character) const;
	static ID3D11ShaderResourceView* TextSystem::ExtractResourceViewsFrom(Texture* texture);

public:
	TextSystem(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, XMMATRIX viewMatrix, Box screenSize);
	~TextSystem();
	void Shutdown() override;

	void Update(vector<Entity*>& entities, float delta) override;
	void Render(vector<Entity*>& entities) override;
};
