#pragma once
#include <vector>
#include <Windows.h>
#include <dirent.h>

#include "engine/font_engine/Font.h"

using namespace std;

class FontEngine
{
private:
	char* _defaultSearchPath = "../../fonts/";
	vector<Font> _avaliableFonts;
private:
	bool FindFontsFolder();
	bool PopulateAvaliableFonts();
public:
	FontEngine();
	~FontEngine();

	bool SearchForAvaliableFonts();
};

