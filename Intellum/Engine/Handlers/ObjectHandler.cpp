#include "ObjectHandler.h"
#include "../Objects/Systems/UISystem.h"

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
	_systemList[UI_RENDER_SYSTEM] = new UISystem(direct3D, shaderController, hwnd, camera, Box(1280, 720));
	_systemList[RENDER_SYSTEM] = new RenderSystem(direct3D, shaderController, hwnd, camera);

	for(int i = 0; i < 25; i++)
	{
		Entity* entity = new Entity();
		TransformComponent* transformComponent = new TransformComponent();
		transformComponent->Position = XMFLOAT3(((static_cast<float>(rand()) / RAND_MAX) * 30.0f) - 15.0f, ((static_cast<float>(rand()) / RAND_MAX) * 10.0f) - 5.0f, ((static_cast<float>(rand()) / RAND_MAX) * 30.0f) - 15.0f);
		transformComponent->AngularVelocity = XMFLOAT3(0.0f, (static_cast<float>(rand()) / RAND_MAX * 5.0f - 2.5f) * static_cast<float>(XM_PI), 0.0f);
		entity->AddComponent(transformComponent);

		AppearanceComponent* appearanceComponent = new AppearanceComponent();
		appearanceComponent->Model = geometryBuilder.FromFile("data/models/sphere.obj");
		appearanceComponent->Textures = CreateTexture::ListFrom(direct3D, { "data/images/stone.tga", "data/images/dirt.tga" });
		entity->AddComponent(appearanceComponent);

		FrustrumCullingComponent* frustrum = new FrustrumCullingComponent();
		frustrum->CullingType = FRUSTRUM_CULL_SPHERE;
		entity->AddComponent(frustrum);

		_entityList.push_back(entity);
	}

	Entity* entity = new Entity();
	entity->AddComponent(new TransformComponent());

	AppearanceComponent* appearanceComponent = new AppearanceComponent();
	appearanceComponent->Model = geometryBuilder.ForGrid(Box(100, 100), XMFLOAT2(10, 10));
	appearanceComponent->Textures = CreateTexture::ListFrom(direct3D, { "data/images/stone.tga", "data/images/dirt.tga" });
	entity->AddComponent(appearanceComponent);

	FrustrumCullingComponent* frustrum = new FrustrumCullingComponent();
	frustrum->CullingType = FRUSTRUM_CULL_RECTANGLE;
	entity->AddComponent(frustrum);

	_entityList.push_back(entity);
	
	Entity* skyBox = new Entity();

	TransformComponent* transform = new TransformComponent();
	transform->Scale = XMFLOAT3(1000, 1000, 1000);
	skyBox->AddComponent(transform);

	RasterizerComponent* rasterizer = new RasterizerComponent();
	rasterizer->CullMode = D3D11_CULL_FRONT;
	skyBox->AddComponent(rasterizer);

	AppearanceComponent* skyBoxAppearance = new AppearanceComponent();
	skyBoxAppearance->Model = geometryBuilder.FromFile("data/models/sphere.obj");
	skyBoxAppearance->Textures = CreateTexture::ListFrom(direct3D, { "data/images/stone.tga" });
	skyBox->AddComponent(skyBoxAppearance);

	_entityList.push_back(skyBox);

	Entity* ui = new Entity();

	AppearanceComponent* uiAppearance = new AppearanceComponent();
	uiAppearance->ShaderType = SHADER_UI;
	uiAppearance->Model = geometryBuilder.ForUI();
	uiAppearance->Textures = CreateTexture::ListFrom(direct3D, { "data/images/dirt.tga", "data/images/josh.tga", "data/images/stone.tga" });
	uiAppearance->LightMap = CreateTexture::From(direct3D, "data/images/basic_light_map.tga");
	ui->AddComponent(uiAppearance);

	TransformComponent* uiTransform = new TransformComponent();
	uiTransform->Position = XMFLOAT3(50, 150, 0);
	ui->AddComponent(uiTransform);

	UIComponent* uiComponent = new UIComponent();
	uiComponent->BitmapSize = XMFLOAT2(256, 256);
	ui->AddComponent(uiComponent);
	
	_entityList.push_back(ui);
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