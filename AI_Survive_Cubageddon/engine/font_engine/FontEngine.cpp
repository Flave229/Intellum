#include "FontEngine.h"

FontEngine::FontEngine()
{
}


FontEngine::~FontEngine()
{
}

bool FontEngine::SearchForAvaliableFonts()
{
	bool result;

	result = FindFontsFolder();
	if (!result) return false;

	vector<string> potentialFonts = GetPotentialFonts();
	if (potentialFonts.size() <= 0) return false;

	vector<string> fonts = ValidatePotentialFonts(potentialFonts);
	if (fonts.size() <= 0) return false;

	

	return true;
}

bool FontEngine::FindFontsFolder()
{
	DIR* directory = opendir(".");
	struct dirent* entry = readdir(directory);
	bool fontsFolderFound = false;

	while (entry != nullptr)
	{
		if (entry->d_type == DT_DIR)
		{
			string subDirectory = (string)entry->d_name;

			if (subDirectory.find("fonts") != string::npos)
			{
				closedir(directory);
				return true;
			}
		}

		entry = readdir(directory);
	}

	closedir(directory);
	return false;
}

vector<string> FontEngine::GetPotentialFonts()
{
	vector<string> fonts = vector<string>();

	DIR* directory = opendir("./fonts");

	struct dirent* entry = readdir(directory);
	bool fontsFolderFound = false;

	while (entry != nullptr)
	{
		if (entry->d_type == DT_DIR)
		{
			string subDirectory = (string)entry->d_name;

			fonts.push_back(subDirectory);
		}

		entry = readdir(directory);
	}
	
	closedir(directory);

	return fonts;
}

vector<string> FontEngine::ValidatePotentialFonts(vector<string> potentialFonts)
{
	vector<string> validatedFonts = vector<string>();

	for (int i = 0; i < potentialFonts.size(); i++)
	{
		string filePath = "./fonts/" + potentialFonts.at(i);
		DIR* directory = opendir(filePath.c_str());

		struct dirent* entry = readdir(directory);
		bool lowerCaseAlphabetFound = false;

		while (entry != nullptr)
		{
			string subDirectory = (string)entry->d_name;

			if (subDirectory.find("alphabet_lower") != string::npos)
			{
				lowerCaseAlphabetFound = true;
				validatedFonts.push_back(potentialFonts.at(i));
			}

			entry = readdir(directory);
		}

		closedir(directory);
	}

	return validatedFonts;
}

bool FontEngine::CreateFonts(vector<string> fontFiles)
{
	
}