#include "ObjectHandler.h"
#include "../Objects/Appearance/GridAppearance.h"

ObjectHandler::ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController, Frustrum* frustrum) : _frustrum(frustrum)
{
	InitialiseObjects(direct3D, shaderController);
}

ObjectHandler::~ObjectHandler()
{
}

void ObjectHandler::InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	for(int i = 0; i < 25; i++)
	{
		Transform* transform = new Transform(direct3D);
		if (!transform) throw Exception("Failed to create a Transform object.");

		transform->SetPosition(XMFLOAT3(((static_cast<float>(rand()) / RAND_MAX) * 30.0f) - 15.0f, ((static_cast<float>(rand()) / RAND_MAX) * 10.0f) - 5.0f, ((static_cast<float>(rand()) / RAND_MAX) * 30.0f) - 15.0f));
		transform->SetAngularVelocity(XMFLOAT3(0.0f, (static_cast<float>(rand()) / RAND_MAX * 5.0f - 2.5f) * static_cast<float>(XM_PI), 0.0f));

		IAppearance* appearance = new Appearance(direct3D, vector<char*> { "data/images/stone.tga", "data/images/dirt.tga" }, "", "data/models/sphere.obj");
		if (!appearance) throw Exception("Failed to create a Appearance object.");

		SceneObject* object = new SceneObject(direct3D, transform, appearance, shaderController->GetShader(SHADER_DEFAULT));
		if (!object) throw Exception("Failed to create a object.");

		_objectList.push_back(object);
	}

	Transform* transform = new Transform(direct3D);
	if (!transform) throw Exception("Failed to create a Transform object.");

	IAppearance* appearance = new GridAppearance(direct3D, vector<char*> { "data/images/stone.tga", "data/images/dirt.tga" }, "", Box(100, 100), XMFLOAT2(10, 10));
	if (!appearance) throw Exception("Failed to create a Appearance for the grid.");

	SceneObject* object = new SceneObject(direct3D, transform, appearance, shaderController->GetShader(SHADER_DEFAULT));
	if (!object) throw Exception("Failed to create a object.");

	_objectList.push_back(object);
}

void ObjectHandler::Shutdown()
{
	for (unsigned long long i = _objectList.size(); i > 0 ; i--)
	{
		_objectList.back()->Shutdown();
		_objectList.pop_back();
	}

	_objectList.clear();
}

void ObjectHandler::Update(float delta)
{
	for(int i = 0; i < _objectList.size(); i++)
	{
		_objectList.at(i)->Update(delta);
	}
}

void ObjectHandler::Render()
{
	_renderCount = 0;
	for (int i = 0; i < _objectList.size(); i++)
	{
		if (_frustrum->CheckSphereInsideFrustrum(_objectList.at(i)->GetTransform()->GetPosition(), 0.5f))
		{
			_renderCount++;
			_objectList.at(i)->Render();
		}
	}
}

int ObjectHandler::GetRenderedModelCount() const
{
	return _renderCount;
}