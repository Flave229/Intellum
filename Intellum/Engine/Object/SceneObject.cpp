#include "SceneObject.h"

SceneObject::SceneObject(DirectX3D* direct3D, Transform* transform, Appearance* appearance, IShaderType* shader) : _direct3D(direct3D), _shader(shader), _transform(transform), _appearance(appearance)
{
}

SceneObject::SceneObject(const SceneObject& other) : SceneObject(other._direct3D, other._transform, other._appearance, other._shader)
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::Update(float delta) const
{
	_transform->SetAngularVelocity(XMFLOAT3(0.0f, static_cast<float>(XM_PI) * 0.5f, 0.0f));

	_transform->Update(delta);
}

void SceneObject::Render() const
{
	_appearance->Render();

	XMMATRIX projectionMatrix;
	_direct3D->MapProjectionMatrixInto(projectionMatrix);

	_shader->Render(_appearance->GetIndexCount(), _transform->GetTransformation(), projectionMatrix, _appearance->GetTexture());
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
