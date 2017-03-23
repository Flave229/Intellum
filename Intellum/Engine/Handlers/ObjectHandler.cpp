#include "ObjectHandler.h"
#include "../Objects/Components/FurstrumCullingComponent.h"

ObjectHandler::ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Camera* camera, Light* light)
{
	InitialiseObjects(direct3D, shaderController, hwnd, camera, light);
}

ObjectHandler::~ObjectHandler()
{
}

void ObjectHandler::Shutdown()
{
	for (unsigned long long i = _entityList.size(); i > 0; i--)
	{
		_entityList.back()->Shutdown();
		_entityList.pop_back();
	}

	_entityList.clear();

	map<SystemType, ISystem*>::iterator iterator;
	for (iterator = _systemList.begin(); iterator != _systemList.end(); ++iterator)
	{
		iterator->second->Shutdown();
		delete iterator->second;
		iterator->second = nullptr;
	}

	_systemList.clear();
}

void ObjectHandler::InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Camera* camera, Light* light)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	GeometryBuilder geometryBuilder = GeometryBuilder(direct3D->GetDevice());

	_systemList[TRANSFORM_SYSTEM] = new TransformSystem(direct3D);
	_systemList[RENDER_SYSTEM] = new RenderSystem(direct3D, hwnd, camera, light);

	for(int i = 0; i < 25; i++)
	{
		Entity* entity = new Entity();
		TransformComponent* transformComponent = new TransformComponent();
		transformComponent->Position = XMFLOAT3(((static_cast<float>(rand()) / RAND_MAX) * 30.0f) - 15.0f, ((static_cast<float>(rand()) / RAND_MAX) * 10.0f) - 5.0f, ((static_cast<float>(rand()) / RAND_MAX) * 30.0f) - 15.0f);
		transformComponent->AngularVelocity = XMFLOAT3(0.0f, (static_cast<float>(rand()) / RAND_MAX * 5.0f - 2.5f) * static_cast<float>(XM_PI), 0.0f);
		entity->AddComponent(transformComponent);

		Geometry modelData = geometryBuilder.FromFile("data/models/sphere.obj");
		IComponent* appearanceComponent = new AppearanceComponent(modelData, CreateTexture::ListFrom(direct3D, { "data/images/stone.tga", "data/images/dirt.tga" }), nullptr);
		entity->AddComponent(appearanceComponent);

		FrustrumCullingComponent* frustrum = new FrustrumCullingComponent();
		frustrum->CullingType = FRUSTRUM_CULL_SPHERE;
		entity->AddComponent(frustrum);

		_entityList.push_back(entity);
	}

	Entity* entity = new Entity();
	entity->AddComponent(new TransformComponent());

	Geometry modelData = geometryBuilder.ForGrid(Box(100, 100), XMFLOAT2(10, 10));
	entity->AddComponent(new AppearanceComponent(modelData, CreateTexture::ListFrom(direct3D, { "data/images/stone.tga", "data/images/dirt.tga" }), nullptr));

	_entityList.push_back(entity);
	
	Entity* skyBox = new Entity();

	TransformComponent* transform = new TransformComponent();
	transform->Scale = XMFLOAT3(1000, 1000, 1000);
	skyBox->AddComponent(transform);

	RasterizerComponent* rasterizer = new RasterizerComponent();
	rasterizer->CullMode = D3D11_CULL_FRONT;
	skyBox->AddComponent(rasterizer);

	Geometry skyBoxModel = geometryBuilder.FromFile("data/models/sphere.obj");
	skyBox->AddComponent(new AppearanceComponent(skyBoxModel, CreateTexture::ListFrom(direct3D, { "data/images/stone.tga" }), nullptr));

	_entityList.push_back(skyBox);
}

void ObjectHandler::Update(float delta)
{
	map<SystemType, ISystem*>::iterator iterator;
	for (iterator = _systemList.begin(); iterator != _systemList.end(); ++iterator)
	{
		iterator->second->Update(_entityList, delta);
	}
}

void ObjectHandler::Render()
{
	map<SystemType, ISystem*>::iterator iterator;
	for (iterator = _systemList.begin(); iterator != _systemList.end(); ++iterator)
	{
		iterator->second->Render(_entityList);
	}
}

int ObjectHandler::GetRenderedModelCount()
{
	return static_cast<RenderSystem*>(_systemList[RENDER_SYSTEM])->RenderCount();
}