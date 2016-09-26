#pragma once
#include <vector>
#include <Windows.h>
#include <dirent.h>

#include "../../error_handling/Exception.h"
#include "Font.h"
#include "Character.h"
#include "../../ShaderController.h"

using namespace std;

class FontEngine
{
private:
	ShaderController* _shader;

	char* _defaultSearchPath = "../../fonts/";
	vector<Font*> _avaliableFonts;
private:
	bool FindFontsFolder();
	vector<string> GetPotentialFonts();
	vector<string> ValidatePotentialFonts(vector<string> potentialFonts);

	bool CreateFonts(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<string> fontFiles, int screenWidth, int screenHeight);
	vector<Character*> GetCharactersFromFontFolder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, string filePath, int screenWidth, int screenHeight);
	Character* CreateCharacterFromFontFolder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, string filePath, string name, string unicode, int screenWidth, int screenHeight);

	vector<Character*> StringToCharacterTextureList(string font, string input);
public:
	FontEngine(ShaderController* shader);
	~FontEngine();

	bool SearchForAvaliableFonts(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight);
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix,
		XMFLOAT3 lightDirection, XMFLOAT3 cameraPosition, XMFLOAT4 lightAmbient, XMFLOAT4 lightDiffuse, XMFLOAT4 lightSpecular,
		float lightSpecularPower, int positionX, int positionY, string font, string input);

	Font* GetFont(string font);
	bool CheckFontExists(string font);
};

