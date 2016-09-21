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
		if (potentialFonts.at(i) == "." || potentialFonts.at(i) == "..")
			continue;

		string filePath = "./fonts/" + potentialFonts.at(i);
		DIR* directory = opendir(filePath.c_str());

		struct dirent* entry = readdir(directory);
		bool lowerAlphabetFound[26];

		for (int i2 = 0; i2 < 26; i2++)
		{
			lowerAlphabetFound[i2] = false;
		}

		while (entry != nullptr)
		{
			string subDirectory = (string)entry->d_name;

			#pragma region CheckAllLowerCaseAlphabetCharactersExist

			if (subDirectory.find("0061") != string::npos)
			{
				lowerAlphabetFound[0] = true;
			}
			else if (subDirectory.find("0062") != string::npos)
			{
				lowerAlphabetFound[1] = true;
			}
			else if (subDirectory.find("0063") != string::npos)
			{
				lowerAlphabetFound[2] = true;
			}
			else if (subDirectory.find("0064") != string::npos)
			{
				lowerAlphabetFound[3] = true;
			}
			else if (subDirectory.find("0065") != string::npos)
			{
				lowerAlphabetFound[4] = true;
			}
			else if (subDirectory.find("0066") != string::npos)
			{
				lowerAlphabetFound[5] = true;
			}
			else if (subDirectory.find("0067") != string::npos)
			{
				lowerAlphabetFound[6] = true;
			}
			else if (subDirectory.find("0068") != string::npos)
			{
				lowerAlphabetFound[7] = true;
			}
			else if (subDirectory.find("0069") != string::npos)
			{
				lowerAlphabetFound[8] = true;
			}
			else if (subDirectory.find("006A") != string::npos)
			{
				lowerAlphabetFound[9] = true;
			}
			else if (subDirectory.find("006B") != string::npos)
			{
				lowerAlphabetFound[10] = true;
			}
			else if (subDirectory.find("006C") != string::npos)
			{
				lowerAlphabetFound[11] = true;
			}
			else if (subDirectory.find("006D") != string::npos)
			{
				lowerAlphabetFound[12] = true;
			}
			else if (subDirectory.find("006E") != string::npos)
			{
				lowerAlphabetFound[13] = true;
			}
			else if (subDirectory.find("006F") != string::npos)
			{
				lowerAlphabetFound[14] = true;
			}
			else if (subDirectory.find("0070") != string::npos)
			{
				lowerAlphabetFound[15] = true;
			}
			else if (subDirectory.find("0071") != string::npos)
			{
				lowerAlphabetFound[16] = true;
			}
			else if (subDirectory.find("0072") != string::npos)
			{
				lowerAlphabetFound[17] = true;
			}
			else if (subDirectory.find("0073") != string::npos)
			{
				lowerAlphabetFound[18] = true;
			}
			else if (subDirectory.find("0074") != string::npos)
			{
				lowerAlphabetFound[19] = true;
			}
			else if (subDirectory.find("0075") != string::npos)
			{
				lowerAlphabetFound[20] = true;
			}
			else if (subDirectory.find("0076") != string::npos)
			{
				lowerAlphabetFound[21] = true;
			}
			else if (subDirectory.find("0077") != string::npos)
			{
				lowerAlphabetFound[22] = true;
			}
			else if (subDirectory.find("0078") != string::npos)
			{
				lowerAlphabetFound[23] = true;
			}
			else if (subDirectory.find("0079") != string::npos)
			{
				lowerAlphabetFound[24] = true;
			}
			else if (subDirectory.find("007A") != string::npos)
			{
				lowerAlphabetFound[25] = true;
			}

			#pragma endregion

			entry = readdir(directory);
		}

		bool allTrue = true;

		for (int i2 = 0; i2 < 26; i2++)
		{
			if (!lowerAlphabetFound[i2])
			{
				closedir(directory);
				allTrue = false;
				break;
			}
		}

		if (allTrue)
		{
			validatedFonts.push_back(potentialFonts.at(i));
			closedir(directory);
		}
	}

	return validatedFonts;
}

bool FontEngine::CreateFonts(vector<string> fontFiles)
{
	for (int i = 0; i < fontFiles.size(); i++)
	{
		Font* font = new Font();

		vector<Character*> lowerCase = GetLowerCaseCharactersFromFontFolder("./fonts/" + fontFiles.at(i));
	}

	return true;
}


vector<Character*> FontEngine::GetLowerCaseCharactersFromFontFolder(string filePath)
{
	return vector<Character*>();
}