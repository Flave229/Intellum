#include "ObjectHandler.h"
#include "../Objects/Appearance/GridAppearance.h"
#include "../Objects/Appearance/SkyBoxAppearance.h"
#include "../Objects/Entity.h"
#include "../Objects/Components/TransformComponent.h"

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

	_transformSystem = new TransformSystem(direct3D);

	_entitySpike.push_back(new Entity());
	IComponent* transformComponent = new TransformComponent(XMFLOAT3(0, 10, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	_entitySpike.at(0)->AddComponent(transformComponent);

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
	
	Transform* transformSkybox = new Transform(direct3D);
	if (!transformSkybox) throw Exception("Failed to create a Transform object.");
	transformSkybox->SetScale(XMFLOAT3(1000, 1000, 1000));

	IAppearance* appearanceSkybox = new SkyBoxAppearance(direct3D, vector<char*> { "data/images/stone.tga" }, "data/models/sphere.obj");
	if (!appearanceSkybox) throw Exception("Failed to create a Appearance for the grid.");

	SceneObject* objectSkybox = new SceneObject(direct3D, transformSkybox, appearanceSkybox, shaderController->GetShader(SHADER_DEFAULT));
	if (!objectSkybox) throw Exception("Failed to create a object.");
	_objectList.push_back(objectSkybox);
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

	_transformSystem->Update(_entitySpike, delta);
}

void ObjectHandler::Render()
{
	_renderCount = 0;
	for (int i = 0; i < _objectList.size(); i++)
	{
		if (_frustrum->CheckSphereInsideFrustrum(_objectList.at(i)->GetTransform()->GetPosition(), 0.5f * _objectList.at(i)->GetTransform()->GetScale().x))
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