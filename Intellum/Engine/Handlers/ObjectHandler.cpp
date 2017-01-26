#include "ObjectHandler.h"

ObjectHandler::ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController)
{
	InitialiseObjects(direct3D, shaderController);
}

ObjectHandler::ObjectHandler(const ObjectHandler& other) : _objectList(other._objectList)
{
}

ObjectHandler::~ObjectHandler()
{
}

void ObjectHandler::InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController)
{
	Transform* transform = new Transform(direct3D);
	if (!transform) throw Exception("Failed to create a Transform object.");

	Appearance* appearance = new Appearance(direct3D, "data/images/stone.tga", "data/models/sphere.obj");
	if (!appearance) throw Exception("Failed to create a Appearance object.");

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
	for (int i = 0; i < _objectList.size(); i++)
	{
		_objectList.at(i)->Render();
	}
}

int ObjectHandler::GetModelCount() const
{
	return _objectList.size();
}
