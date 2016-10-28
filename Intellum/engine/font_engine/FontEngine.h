#pragma once
#include <vector>
#include <Windows.h>
#include <dirent.h>

#include "../../error_handling/Exception.h"
#include "Font.h"
#include "Character.h"
#include "../shader_engine/IShaderType.h"
#include "../shader_engine/FontShader.h"
#include "../DirectX3D.h"

using namespace std;

class FontEngine
{
private:
	DirectX3D* _direct3D;

	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	IShaderType* _shader;

	char* _defaultSearchPath = "../../fonts/";
	vector<Font*> _avaliableFonts;
private:
	bool FindFontsFolder();
	vector<string> GetPotentialFonts();
	vector<string> ValidatePotentialFonts(vector<string> potentialFonts);

	bool CreateFonts(vector<string> fontFiles, int screenWidth, int screenHeight);
	vector<Character*> GetCharactersFromFontFolder(string filePath, int screenWidth, int screenHeight);
	bool CheckCharacterExists(string filePath);
	Character* CreateCharacterFromFontFolder(string filePath, string name, string unicode, int screenWidth, int screenHeight);

	vector<Character*> StringToCharacterTextureList(string font, string input);
public:
	FontEngine(DirectX3D* direct3D, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IShaderType* shader);
	~FontEngine();

	bool SearchForAvaliableFonts(int screenWidth, int screenHeight);
	bool Render(XMMATRIX viewMatrix, XMFLOAT3 cameraPosition, Light* light, int positionX, int positionY, string font, string input, XMFLOAT4 textColor, int fontSize);

	Font* GetFont(string font);
	bool CheckFontExists(string font);
};

