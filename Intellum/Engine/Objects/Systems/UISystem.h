#pragma once
#include "ISystem.h"
#include "../../ShaderEngine/ShaderController.h"
#include "../../../common/Vertex.h"
#include "../Components/UIComponent.h"

class UISystem : public ISystem
{
private:
	Box _screenSize;

public:
	DirectX3D* _direct3D;
	ShaderController* _shaderController;

	UISystem(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Box screenSize);
	~UISystem() override = default;

	void Shutdown() override;
	void Update(vector<Entity*>& entities, float delta) override;
	void Render(vector<Entity*>& entities) override;
};