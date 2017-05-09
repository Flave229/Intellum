#include "ObjectHandler.h"
#include "../Objects/Components/ButtonComponent.h"
#include "../Objects/Systems/ButtonSystem.h"
#include "../Objects/Commands/ExitApplicationCommand.h"
#include "../Objects/Components/InputComponent.h"
#include "../Objects/Commands/ToggleVisibleCommand.h"
#include "../Objects/Systems/InputSystem.h"

ObjectHandler::ObjectHandler(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, HWND hwnd, Camera* camera, Input* input, FramesPerSecond* framesPerSecond, Cpu* cpu, Box screenSize)
{
	InitialiseObjects(direct3D, shaderController, fontEngine, hwnd, camera, input, framesPerSecond, cpu, screenSize);
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

void ObjectHandler::InitialiseObjects(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, HWND hwnd, Camera* camera, Input* input, FramesPerSecond* framesPerSecond, Cpu* cpu, Box screenSize)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	GeometryBuilder geometryBuilder = GeometryBuilder(direct3D->GetDevice());

	ButtonSystem* buttonSystem = new ButtonSystem(input);

	_systemList[TRANSFORM_SYSTEM] = new TransformSystem(direct3D);
	_systemList[UI_RENDER_SYSTEM] = new UISystem(direct3D, shaderController, hwnd, screenSize);
	_systemList[RENDER_SYSTEM] = new RenderSystem(direct3D, shaderController, hwnd, camera);
	_systemList[FONT_SYSTEM] = new TextSystem(direct3D, shaderController, fontEngine, camera->GetViewMatrix(), screenSize);
	_systemList[BUTTON_SYSTEM] = buttonSystem;
	_systemList[INPUT_SYSTEM] = new InputSystem(input);

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
	skyBoxAppearance->Gradient = GradientShaderParameters(XMFLOAT4(0.49f, 0.75f, 0.93f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
	skyBox->AddComponent(skyBoxAppearance);

	_entityList.push_back(skyBox);

	Entity* text1 = new Entity();
	TextComponent* textComponent1 = new TextComponent();
	textComponent1->Text = "Victoria Grump xxx";
	textComponent1->FontSize = 30;
	textComponent1->FontPosition = XMFLOAT2(50, 600);
	textComponent1->Color = XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f);
	text1->AddComponent(textComponent1);

	_entityList.push_back(text1);

	Entity* text2 = new Entity();
	TextComponent* textComponent2 = new TextComponent();
	textComponent2->Text = "Mouse X:    Mouse Y: ";
	textComponent2->FontSize = 20;
	textComponent2->FontPosition = XMFLOAT2(10, 40);
	textComponent2->Color = XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f);
	text2->AddComponent(textComponent2);

	_entityList.push_back(text2);
	input->AddObserver(textComponent2);

	Entity* text3 = new Entity();
	TextComponent* textComponent3 = new TextComponent();
	textComponent3->Text = "FPS: 0";
	textComponent3->FontSize = 20;
	textComponent3->FontPosition = XMFLOAT2(10, 65);
	textComponent3->Color = XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f);
	text3->AddComponent(textComponent3);

	_entityList.push_back(text3);
	framesPerSecond->AddObserver(textComponent3);

	Entity* text4 = new Entity();
	TextComponent* textComponent4 = new TextComponent();
	textComponent4->Text = "CPU: 0%";
	textComponent4->FontSize = 20;
	textComponent4->FontPosition = XMFLOAT2(10, 90);
	textComponent4->Color = XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f);
	text4->AddComponent(textComponent4);

	_entityList.push_back(text4);
	cpu->AddObserver(textComponent4);

	Entity* text5 = new Entity();
	TextComponent* textComponent5 = new TextComponent();
	textComponent5->Text = "Rendered: 0";
	textComponent5->FontSize = 20;
	textComponent5->FontPosition = XMFLOAT2(10, 115);
	textComponent5->Color = XMFLOAT4(0.6f, 0.0f, 0.6f, 1.0f);
	text5->AddComponent(textComponent5);

	_entityList.push_back(text5);
	static_cast<RenderSystem*>(_systemList[RENDER_SYSTEM])->AddObserver(textComponent5);

	Entity* ui = new Entity();

	AppearanceComponent* uiAppearance = new AppearanceComponent();
	uiAppearance->ShaderType = SHADER_UI;
	uiAppearance->Model = geometryBuilder.ForUI();
	uiAppearance->Textures = CreateTexture::ListFrom(direct3D, { "data/images/dirt.tga", "data/images/josh.tga", "data/images/stone.tga" });
	uiAppearance->LightMap = CreateTexture::From(direct3D, "data/images/basic_light_map.tga");
	ui->AddComponent(uiAppearance);

	TransformComponent* uiTransform = new TransformComponent();
	uiTransform->Position = XMFLOAT3(10, 150, 0);
	ui->AddComponent(uiTransform);

	UIComponent* uiComponent = new UIComponent();
	uiComponent->BitmapSize = XMFLOAT2(256, 256);
	ui->AddComponent(uiComponent);

	_entityList.push_back(ui);

	Entity* navigationBar = new Entity();

	AppearanceComponent* navigationBarAppearance = new AppearanceComponent();
	navigationBarAppearance->ShaderType = SHADER_UI;
	navigationBarAppearance->Model = geometryBuilder.ForUI();
	navigationBarAppearance->Color = ColorShaderParameters(XMFLOAT4(0.5, 0.5, 0.5, 1));
	navigationBar->AddComponent(navigationBarAppearance);

	TransformComponent* navigationBarTransform = new TransformComponent();
	navigationBarTransform->Position = XMFLOAT3(0, 0, 2);
	navigationBar->AddComponent(navigationBarTransform);

	UIComponent* navigationBarComponent = new UIComponent();
	navigationBarComponent->BitmapSize = XMFLOAT2(screenSize.Width, 30);
	navigationBar->AddComponent(navigationBarComponent);

	InputComponent* navigationBarInput = new InputComponent();
	navigationBarInput->ControlCommands[ESCAPE] = new ToggleVisibleCommand(navigationBarAppearance);
	navigationBar->AddComponent(navigationBarInput);

	_entityList.push_back(navigationBar); 
	
	Entity* button1 = new Entity();

	AppearanceComponent* button1Appearance = new AppearanceComponent();
	button1Appearance->ShaderType = SHADER_UI;
	button1Appearance->Model = geometryBuilder.ForUI();
	button1Appearance->Color = ColorShaderParameters(XMFLOAT4(0.4, 0.4, 0.4, 1));
	button1->AddComponent(button1Appearance);

	TransformComponent* button1Transform = new TransformComponent();
	button1Transform->Position = XMFLOAT3(0, 0, 1);
	button1->AddComponent(button1Transform);

	UIComponent* button1Component = new UIComponent();
	button1Component->BitmapSize = XMFLOAT2(170, 30);
	button1->AddComponent(button1Component);

	TextComponent* button1Text = new TextComponent();
	button1Text->Text = "Graphics";
	button1Text->FontSize = 20;
	button1Text->FontPosition = XMFLOAT2(0, 3);
	button1Text->Color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	button1->AddComponent(button1Text);

	ButtonComponent* button1Button = new ButtonComponent();
	button1->AddComponent(button1Button);

	InputComponent* button1Input = new InputComponent();
	button1Input->ControlCommands[ESCAPE] = new ToggleVisibleCommand(button1Appearance);
	button1->AddComponent(button1Input);

	_entityList.push_back(button1);

	Entity* button2 = new Entity();

	AppearanceComponent* button2Appearance = new AppearanceComponent();
	button2Appearance->ShaderType = SHADER_UI;
	button2Appearance->Model = geometryBuilder.ForUI();
	button2Appearance->Color = ColorShaderParameters(XMFLOAT4(0.4, 0.4, 0.4, 1));
	button2->AddComponent(button2Appearance);

	TransformComponent* button2Transform = new TransformComponent();
	button2Transform->Position = XMFLOAT3(175, 0, 1);
	button2->AddComponent(button2Transform);

	UIComponent* button2Component = new UIComponent();
	button2Component->BitmapSize = XMFLOAT2(170, 30);
	button2->AddComponent(button2Component);

	TextComponent* button2Text = new TextComponent();
	button2Text->Text = "Exit";
	button2Text->FontSize = 20;
	button2Text->FontPosition = XMFLOAT2(175, 3);
	button2Text->Color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	button2->AddComponent(button2Text);

	ButtonComponent* button2Button = new ButtonComponent();
	button2Button->OnClickCommand = new ExitApplicationCommand();
	button2->AddComponent(button2Button);

	InputComponent* button2Input = new InputComponent();
	button2Input->ControlCommands[ESCAPE] = new ToggleVisibleCommand(button2Appearance);
	button2->AddComponent(button2Input);

	_entityList.push_back(button2);

	Entity* cursor = new Entity();

	AppearanceComponent* cursorAppearance = new AppearanceComponent();
	cursorAppearance->ShaderType = SHADER_UI;
	cursorAppearance->Model = geometryBuilder.ForUI();
	cursorAppearance->Textures = CreateTexture::ListFrom(direct3D, { "data/images/cursor.tga" });
	cursor->AddComponent(cursorAppearance);

	TransformComponent* cursorTransform = new TransformComponent();
	cursorTransform->Position = XMFLOAT3(400, 400, 0);
	cursor->AddComponent(cursorTransform);

	UIComponent* cursorComponent = new UIComponent();
	cursorComponent->BitmapSize = XMFLOAT2(24, 24);
	cursor->AddComponent(cursorComponent);

	InputComponent* cursorInput = new InputComponent();
	cursorInput->ControlCommands[ESCAPE] = new ToggleVisibleCommand(cursorAppearance);
	cursor->AddComponent(cursorInput);

	_entityList.push_back(cursor);
	input->AddObserver(cursorTransform);
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