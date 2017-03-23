#pragma once
#include "../ShaderEngine/IShaderType.h"
#include "../Camera/Frustrum.h"
#include "ISceneObject.h"
#include "Appearance/IAppearance.h"

class SceneObject : public ISceneObject
{
private:
	DirectX3D* _direct3D;

	IShaderType* _shader;
	Transform* _transform;
	IAppearance* _appearance;
public:
	SceneObject(DirectX3D* direct3D, Transform* transform, IAppearance* appearance, IShaderType* shader);
	~SceneObject() = default;

	void Update(float delta) override;
	void Render() override;

	Transform* GetTransform() override;

	void Shutdown() override;
};
