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
	vector<SceneObject*> _objectList;

	void InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController);
public:
	ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController);
	ObjectHandler(const ObjectHandler& other);
	~ObjectHandler();

	void Shutdown();

	void Update(float delta);
	void Render();

	int GetModelCount() const;
	void GetData(XMFLOAT4& color, XMFLOAT3& position, float index);
};