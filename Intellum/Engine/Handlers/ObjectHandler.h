#pragma once

#include <DirectXMath.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "../Objects/SceneObject.h"
#include "../ShaderEngine/ShaderController.h"
#include "../Objects/Entity.h"
#include "../Objects/Systems/TransformSystem.h"
#include "../Objects/Systems/RenderSystem.h"

using namespace DirectX;
using namespace std;

class ObjectHandler
{
private:
	Frustrum* _frustrum;
	vector<ISceneObject*> _objectList;

	int _renderCount;

	vector<Entity*> _entitySpike;
	TransformSystem* _transformSystem;
	RenderSystem* _renderSystem;
	void InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Camera* camera, Light* light);
public:
	ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController, Frustrum* frustrum, HWND hwnd, Camera* camera, Light* light);
	~ObjectHandler();

	void Shutdown();

	void Update(float delta);
	void Render();

	int GetRenderedModelCount() const;
};