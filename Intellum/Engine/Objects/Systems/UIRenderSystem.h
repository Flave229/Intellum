#pragma once
#include "ISystem.h"
#include "../../ShaderEngine/ShaderController.h"
#include "../Components/UIAppearanceComponent.h"
#include "../../../common/Vertex.h"
#include "../Components/UIComponent.h"

class UIRenderSystem : public ISystem
{
private:
	Box _screenSize;

	void BuildBufferInformation(Entity* entity, UIAppearanceComponent* appearance) const;
	static vector<ID3D11ShaderResourceView*> ExtractResourceViewsFrom(vector<Texture*> textures);
public:
	DirectX3D* _direct3D;
	Camera* _camera;
	ShaderController* _shaderController;

	UIRenderSystem(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Camera* camera, Box screenSize);
	~UIRenderSystem() override = default;

	void Shutdown() override;
	void Update(vector<Entity*> entities, float delta) override;
	void Render(vector<Entity*> entities) override;
};

