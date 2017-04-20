#pragma once
#include <dirent.h>
#include <string>
#include <vector>

using namespace std;

class FontRetriever
{
private:
	static bool FindFontsFolder();
	static vector<string> GetPotentialFonts();
	static vector<string> ValidatePotentialFonts(vector<string> potentialFonts);
public:
	FontRetriever();
	~FontRetriever();

	static vector<string> RetrieveAvaliableFontNames();
};