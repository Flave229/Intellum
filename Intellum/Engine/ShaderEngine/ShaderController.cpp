#include "ShaderController.h"

ShaderController::ShaderController(DirectX3D* direct3D) : _direct3D(direct3D), _shaders(map<ShaderType, IShaderType*>())
{
}

ShaderController::~ShaderController()
{
}

bool ShaderController::Initialise(HWND hwnd, Camera* camera, Light* light)
{
	_shaders[SHADER_DEFAULT] = new DefaultShader(_direct3D, camera, light);
	if (!_shaders[SHADER_DEFAULT]) throw Exception("Failed to create the default shader.");

	_shaders[SHADER_DEFAULT]->Initialise(hwnd);

	_shaders[SHADER_FONT] = new UIShader(_direct3D, camera, light);
	if (!_shaders[SHADER_FONT]) throw Exception("Failed to create the font shader.");

	_shaders[SHADER_FONT]->Initialise(hwnd);

	_shaders[SHADER_UI] = new DefaultShader(_direct3D, camera, light);
	if (!_shaders[SHADER_UI]) throw Exception("Failed to create the ui shader.");

	_shaders[SHADER_UI]->Initialise(hwnd);
	return true;
}

void ShaderController::Shutdown()
{
	for (map<ShaderType, IShaderType*>::iterator iterator = _shaders.begin(); iterator != _shaders.end(); ++iterator)
	{
		if(iterator->second != nullptr)
		{
			iterator->second->Shutdown();
			delete iterator->second;
			iterator->second = nullptr;
		}
	}

	_shaders.clear();
}

IShaderType* ShaderController::GetShader(ShaderType type)
{
	return _shaders[type];
}