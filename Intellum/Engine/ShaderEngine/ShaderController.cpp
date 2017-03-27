#include "ShaderController.h"

ShaderController::ShaderController(DirectX3D* direct3D) : _direct3D(direct3D), _defaultShader(nullptr), _fontShader(nullptr)
{
}

ShaderController::ShaderController(const ShaderController& other) : _direct3D(other._direct3D), _fontShader(other._fontShader)
{
}

ShaderController::~ShaderController()
{
}

bool ShaderController::Initialise(HWND hwnd, Camera* camera, Light* light)
{
	_defaultShader = new DefaultShader(_direct3D, camera, light);
	if (!_defaultShader) throw Exception("Failed to create the default shader.");

	_defaultShader->Initialise(hwnd);

	_fontShader = new FontShader(_direct3D, camera, light);
	if (!_fontShader) throw Exception("Failed to create the font shader.");

	_fontShader->Initialise(hwnd);

	return true;
}

void ShaderController::Shutdown()
{
	if (_defaultShader)
	{
		_defaultShader->Shutdown();
		delete _defaultShader;
		_defaultShader = nullptr;
	}

	if (_fontShader)
	{
		_fontShader->Shutdown();
		delete _fontShader;
		_fontShader = nullptr;
	}
}

IShaderType* ShaderController::GetShader(ShaderType type)
{
	switch (type)
	{
	case SHADER_DEFAULT:
		return _defaultShader;
	case SHADER_UI:
		return _fontShader;
	default:
		return _defaultShader;
	}
}