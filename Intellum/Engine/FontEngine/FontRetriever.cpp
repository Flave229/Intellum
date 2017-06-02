#include "FontRetriever.h"
#include "../../ErrorHandling/Exception.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <filesystem>

FontRetriever::FontRetriever()
{
}

FontRetriever::~FontRetriever()
{
}

vector<string> FontRetriever::RetrieveAvaliableFontNames()
{
	bool result = FindFontsFolder();
	if (!result) throw Exception("Could not find the font folder to load fonts.");

	vector<string> fonts = GetAllFonts();
	if (fonts.size() <= 0) throw Exception("No fonts could be found in the font folder.");

	return fonts;
}

bool FontRetriever::FindFontsFolder()
{
	struct stat info;

	if (stat("Fonts/", &info) != 0)
		return false;

	return true;
}

vector<string> FontRetriever::GetAllFonts()
{
	vector<string> fonts = vector<string>();

	string path = "Fonts/";
	for (directory_entry p : directory_iterator(path))
	{
		string fontSubDirectory = p.path().generic_string();
		string fontName = fontSubDirectory.erase(0, 6);
		fonts.push_back(fontName);
	}

	return fonts;
}