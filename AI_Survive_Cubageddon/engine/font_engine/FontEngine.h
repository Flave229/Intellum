#pragma once
#include <vector>
#include <Windows.h>
#include <dirent.h>

#include "../../error_handling/Exception.h"
#include "Font.h"
#include "Character.h"

using namespace std;

class FontEngine
{
private:
	char* _defaultSearchPath = "../../fonts/";
	vector<Font*> _avaliableFonts;
private:
	bool FindFontsFolder();
	vector<string> GetPotentialFonts();
	vector<string> ValidatePotentialFonts(vector<string> potentialFonts);
	bool CreateFonts(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<string> fontFiles, int screenWidth, int screenHeight);
	vector<Character*> GetCharactersFromFontFolder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, string filePath, int screenWidth, int screenHeight);
	Character* CreateCharacterFromFontFolder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, string filePath, string name, string unicode, int screenWidth, int screenHeight);
public:
	FontEngine();
	~FontEngine();

	bool SearchForAvaliableFonts(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight);
};

