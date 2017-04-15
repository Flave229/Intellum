#pragma once

#include <DirectXMath.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "../ShaderEngine/ShaderController.h"
#include "../Objects/Entity.h"
#include "../Objects/Systems/TransformSystem.h"
#include "../Objects/Systems/RenderSystem.h"
#include "../Objects/Entity.h"
#include "../Objects/Components/TransformComponent.h"
#include "../Objects/Components/AppearanceComponent.h"
#include "../Objects/Systems/RenderSystem.h"
#include "../Objects/Geometry/GeometryBuilder.h"
#include "../Objects/Components/RasterizerComponent.h"
#include "../Objects/Appearance/Texture/CreateTexture.h"
#include "../Objects/Systems/SystemType.h"
#include "../Objects/Components/FurstrumCullingComponent.h"
#include "../Objects/Systems/UISystem.h"
#include "../Objects/Components/TextComponent.h"
#include "../FontEngine/FontEngine.h"
#include "../Objects/Systems/FontSystem.h"

using namespace DirectX;
using namespace std;

class ObjectHandler
{
private:
	Frustrum* _frustrum;

	vector<Entity*> _entityList;
	map<SystemType, ISystem*> _systemList;
	void InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, HWND hwnd, Camera* camera, Light* light, Input* input);
public:
	ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, HWND hwnd, Camera* camera, Light* light, Input* input);
	~ObjectHandler();

	void Shutdown();

	void Update(float delta);
	void Render();

	int GetRenderedModelCount();
};