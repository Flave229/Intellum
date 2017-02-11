#include "SceneObject.h"

SceneObject::SceneObject(DirectX3D* direct3D, Transform* transform, Appearance* appearance, IShaderType* shader) : _direct3D(direct3D), _shader(shader), _transform(transform), _appearance(appearance)
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::Update(float delta) const
{
	_transform->Update(delta);
}

void SceneObject::Render() const
{
	_appearance->Render();

	ShaderResources shaderResources;
	shaderResources.textureArray = _appearance->GetTextures();
	shaderResources.lightMap = _appearance->GetLightMap();

	_shader->Render(_appearance->GetIndexCount(), shaderResources, _transform->GetTransformation(), _direct3D->GetProjectionMatrix());
}

Transform* SceneObject::GetTransform() const
{
	return _transform;
}

void SceneObject::Shutdown()
{
	if (_transform)
	{
		delete _transform;
		_transform = nullptr;
	}

	if (_appearance)
	{
		delete _appearance;
		_appearance = nullptr;
	}
}