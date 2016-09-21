#pragma once
#include <vector>
#include <Windows.h>
#include <dirent.h>

#include "Font.h"
#include "Character.h"

using namespace std;

class FontEngine
{
private:
	char* _defaultSearchPath = "../../fonts/";
	vector<Font> _avaliableFonts;
private:
	bool FindFontsFolder();
	vector<string> GetPotentialFonts();
	vector<string> ValidatePotentialFonts(vector<string> potentialFonts);
	bool CreateFonts(vector<string> fontFiles);
	vector<Character*> GetLowerCaseCharactersFromFontFolder(string filePath);
public:
	FontEngine();
	~FontEngine();

	bool SearchForAvaliableFonts();
};

