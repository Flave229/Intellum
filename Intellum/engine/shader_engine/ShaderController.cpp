#include "ShaderController.h"

ShaderController::ShaderController(ID3D11Device* device, ID3D11DeviceContext* deviceContext) : _device(device), _deviceContext(deviceContext), _defaultShader(nullptr), _fontShader(nullptr)
{
}

ShaderController::ShaderController(const ShaderController& other) : _device(other._device), _deviceContext(other._deviceContext), _defaultShader(other._defaultShader), _fontShader(other._fontShader)
{
}

ShaderController::~ShaderController()
{
}

bool ShaderController::Initialise(HWND hwnd)
{
	_defaultShader = new DefaultShader(_device, _deviceContext);
	if (!_defaultShader) throw Exception("Failed to create the default shader.");

	bool result = _defaultShader->Initialise(hwnd);
	if (!result) throw Exception("Could not initialise the default shader.");

	_fontShader = new FontShader(_device, _deviceContext);
	if (!_fontShader) throw Exception("Failed to create the default shader.");

	result = _fontShader->Initialise(hwnd);
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