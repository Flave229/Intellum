#include "ObjectHandler.h"

ObjectHandler::ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController, Frustrum* frustrum, HWND hwnd, Camera* camera, Light* light) : _frustrum(frustrum)
{
	InitialiseObjects(direct3D, shaderController, hwnd, camera, light);
}

ObjectHandler::~ObjectHandler()
{
}

void ObjectHandler::InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Camera* camera, Light* light)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	GeometryBuilder geometryBuilder = GeometryBuilder(direct3D->GetDevice());

	_transformSystem = new TransformSystem(direct3D);
	_renderSystem = new RenderSystem(direct3D, hwnd, camera, light);

	for(int i = 0; i < 25; i++)
	{
		Entity* entity = new Entity();
		XMFLOAT3 position = XMFLOAT3(((static_cast<float>(rand()) / RAND_MAX) * 30.0f) - 15.0f, ((static_cast<float>(rand()) / RAND_MAX) * 10.0f) - 5.0f, ((static_cast<float>(rand()) / RAND_MAX) * 30.0f) - 15.0f);
		TransformComponent* transformComponent = new TransformComponent(position, XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
		transformComponent->AngularVelocity = XMFLOAT3(0.0f, (static_cast<float>(rand()) / RAND_MAX * 5.0f - 2.5f) * static_cast<float>(XM_PI), 0.0f);
		entity->AddComponent(transformComponent);

		Geometry modelData = geometryBuilder.FromFile("data/models/sphere.obj");
		IComponent* appearanceComponent = new AppearanceComponent(modelData, CreateTexture::ListFrom(direct3D, { "data/images/stone.tga", "data/images/dirt.tga" }), nullptr);
		entity->AddComponent(appearanceComponent);

		_entityList.push_back(entity);
	}

	Entity* entity = new Entity();
	entity->AddComponent(new TransformComponent());

	Geometry modelData = geometryBuilder.ForGrid(Box(100, 100), XMFLOAT2(10, 10));
	entity->AddComponent(new AppearanceComponent(modelData, CreateTexture::ListFrom(direct3D, { "data/images/stone.tga", "data/images/dirt.tga" }), nullptr));

	_entityList.push_back(entity);
	
	Transform* transformSkybox = new Transform(direct3D);
	if (!transformSkybox) throw Exception("Failed to create a Transform object.");
	transformSkybox->SetScale(XMFLOAT3(1000, 1000, 1000));

	IAppearance* appearanceSkybox = new SkyBoxAppearance(direct3D, vector<char*> { "data/images/stone.tga" }, "data/models/sphere.obj");
	if (!appearanceSkybox) throw Exception("Failed to create a Appearance for the grid.");

	SceneObject* objectSkybox = new SceneObject(direct3D, transformSkybox, appearanceSkybox, shaderController->GetShader(SHADER_DEFAULT));
	if (!objectSkybox) throw Exception("Failed to create a object.");
	_OLD_objectList.push_back(objectSkybox);
}

void ObjectHandler::Shutdown()
{
	for (unsigned long long i = _entityList.size(); i > 0; i--)
	{
		_entityList.back()->Shutdown();
		_entityList.pop_back();
	}

	_entityList.clear();

	for (unsigned long long i = _OLD_objectList.size(); i > 0 ; i--)
	{
		_OLD_objectList.back()->Shutdown();
		_OLD_objectList.pop_back();
	}

	_OLD_objectList.clear();
}

void ObjectHandler::Update(float delta)
{
	for(int i = 0; i < _OLD_objectList.size(); i++)
	{
		_OLD_objectList.at(i)->Update(delta);
	}

	_transformSystem->Update(_entityList, delta);
}

void ObjectHandler::Render()
{
	_renderSystem->Render(_entityList);
	_renderCount = 0;
	for (int i = 0; i < _OLD_objectList.size(); i++)
	{
		if (_frustrum->CheckSphereInsideFrustrum(_OLD_objectList.at(i)->GetTransform()->GetPosition(), 0.5f * _OLD_objectList.at(i)->GetTransform()->GetScale().x))
		{
			_renderCount++;
			_OLD_objectList.at(i)->Render();
		}
	}
}

int ObjectHandler::GetRenderedModelCount() const
{
	return _renderCount;
}