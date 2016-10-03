#include "ShaderController.h"

ShaderController::ShaderController() : _defaultShader(nullptr), _fontShader(nullptr)
{
}

ShaderController::ShaderController(const ShaderController& other) : _defaultShader(other._defaultShader), _fontShader(other._fontShader)
{
}

ShaderController::~ShaderController()
{
}

bool ShaderController::Initialise(ID3D11Device* device, HWND hwnd)
{
	_defaultShader = new DefaultShader();
	if (!_defaultShader) throw Exception("Failed to create the default shader.");

	bool result = _defaultShader->Initialise(device, hwnd);
	if (!result) throw Exception("Could not initialise the default shader.");

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
	case SHADER_FONT:
		return _fontShader;
	default:
		return _defaultShader;
	}
}