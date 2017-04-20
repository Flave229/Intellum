#include "FontRetriever.h"
#include "../../ErrorHandling/Exception.h"

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

	vector<string> fonts = GetPotentialFonts();
	if (fonts.size() <= 0) throw Exception("No fonts could be found in the font folder.");

	fonts = ValidatePotentialFonts(fonts);
	if (fonts.size() <= 0) throw Exception("No valid fonts could be found in the font folder");

	return fonts;
}

bool FontRetriever::FindFontsFolder()
{
	DIR* directory = opendir(".");
	struct dirent* entry = readdir(directory);
	bool fontsFolderFound = false;

	while (entry != nullptr)
	{
		if (entry->d_type == DT_DIR)
		{
			string subDirectory = static_cast<string>(entry->d_name);

			if (subDirectory.find("Fonts") != string::npos)
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

vector<string> FontRetriever::GetPotentialFonts()
{
	vector<string> fonts = vector<string>();

	DIR* directory = opendir("./fonts");

	struct dirent* entry = readdir(directory);
	bool fontsFolderFound = false;

	while (entry != nullptr)
	{
		if (entry->d_type == DT_DIR)
		{
			string subDirectory = static_cast<string>(entry->d_name);

			fonts.push_back(subDirectory);
		}

		entry = readdir(directory);
	}

	closedir(directory);

	return fonts;
}

vector<string> FontRetriever::ValidatePotentialFonts(vector<string> potentialFonts)
{
	vector<string> validatedFonts = vector<string>();

	for (int i = 0; i < potentialFonts.size(); i++)
	{
		if (potentialFonts.at(i) == "." || potentialFonts.at(i) == "..")
			continue;

		validatedFonts.push_back(potentialFonts.at(i));
	}

	return validatedFonts;
}