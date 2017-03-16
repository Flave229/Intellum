#pragma once
#include "Appearance/Appearance.h"
#include "../ShaderEngine/IShaderType.h"
#include "../Camera/Frustrum.h"
#include "ISceneObject.h"

class SceneObject : public ISceneObject
{
private:
	DirectX3D* _direct3D;

	IShaderType* _shader;
	Transform* _transform;
	Appearance* _appearance;
public:
	SceneObject(DirectX3D* direct3D, Transform* transform, Appearance* appearance, IShaderType* shader);
	~SceneObject() = default;

	void Update(float delta) override;
	void Render() override;

	Transform* GetTransform() override;

	void Shutdown() override;
};
