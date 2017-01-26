#pragma once
#include <vector>
#include <Windows.h>
#include <dirent.h>

#include "../../ErrorHandling/Exception.h"
#include "Font.h"
#include "Character.h"
#include "../ShaderEngine/IShaderType.h"
#include "../ShaderEngine/FontShader.h"
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

	bool CreateFonts(vector<string> fontFiles, Box screenSize);
	vector<Character*> GetCharactersFromFontFolder(string filePath, Box screenSize);
	bool CheckCharacterExists(string filePath);
	Character* CreateCharacterFromFontFolder(string filePath, string name, string unicode, Box screenSize);

	vector<Character*> StringToCharacterTextureList(string font, string input);
public:
	FontEngine(DirectX3D* direct3D, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IShaderType* shader);
	~FontEngine();

	void Update(XMFLOAT2 position, string font, string input, XMFLOAT4 textColor, int fontSize);
	
	bool SearchForAvaliableFonts(Box screenSize);

	Font* GetFont(string font);
	bool CheckFontExists(string font);
};
