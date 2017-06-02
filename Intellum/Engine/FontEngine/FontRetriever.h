#pragma once
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
using namespace experimental::filesystem;

class FontRetriever
{
private:
	static bool FindFontsFolder();
	static vector<string> GetAllFonts();
public:
	FontRetriever();
	~FontRetriever();

	static vector<string> RetrieveAvaliableFontNames();
};