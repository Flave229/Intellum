#pragma once
#include "Transform.h"
#include "Appearance.h"
#include "../ShaderEngine/IShaderType.h"

class SceneObject
{
private:
	DirectX3D* _direct3D;

	IShaderType* _shader;
	Transform* _transform;
	Appearance* _appearance;

public:
	SceneObject(DirectX3D* direct3D, Transform* transform, Appearance* appearance, IShaderType* shader);
	SceneObject(const SceneObject& other);
	~SceneObject();

	void Update(float delta) const;
	void Render() const;

	void Shutdown();
};
