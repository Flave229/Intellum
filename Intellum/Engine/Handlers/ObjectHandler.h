#pragma once

#include <DirectXMath.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "../Object/SceneObject.h"
#include "../ShaderEngine/ShaderController.h"

using namespace DirectX;
using namespace std;

class ObjectHandler
{
private:
	Frustrum* _frustrum;
	vector<ISceneObject*> _objectList;

	int _renderCount;

	void InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController);
public:
	ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController, Frustrum* frustrum);
	~ObjectHandler();

	void Shutdown();

	void Update(float delta);
	void Render();

	int GetRenderedModelCount() const;
};