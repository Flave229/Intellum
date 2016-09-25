#include "FontEngine.h"

FontEngine::FontEngine()
{
}


FontEngine::~FontEngine()
{
}

bool FontEngine::SearchForAvaliableFonts(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight)
{
	try
	{
		bool result;

		result = FindFontsFolder();
		if (!result) return false;

		vector<string> potentialFonts = GetPotentialFonts();
		if (potentialFonts.size() <= 0) return false;

		vector<string> fonts = ValidatePotentialFonts(potentialFonts);
		if (fonts.size() <= 0) return false;

		result = CreateFonts(device, deviceContext, fonts, screenWidth, screenHeight);
		if (!result) return false;

		return true;
	}
	catch (Exception& exception)
	{
		throw exception;
	}
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
		bool upperAlphabetFound[26];
		bool lowerAlphabetFound[26];

		for (int i2 = 0; i2 < 26; i2++)
		{
			upperAlphabetFound[i2] = false;
			lowerAlphabetFound[i2] = false;
		}

		while (entry != nullptr)
		{
			string subDirectory = (string)entry->d_name;

			#pragma region CheckAllUpperCaseAlphabetCharactersExist

			if (subDirectory.find("0041") != string::npos)
			{
				upperAlphabetFound[0] = true;
			}
			else if (subDirectory.find("0042") != string::npos)
			{
				upperAlphabetFound[1] = true;
			}
			else if (subDirectory.find("0043") != string::npos)
			{
				upperAlphabetFound[2] = true;
			}
			else if (subDirectory.find("0044") != string::npos)
			{
				upperAlphabetFound[3] = true;
			}
			else if (subDirectory.find("0045") != string::npos)
			{
				upperAlphabetFound[4] = true;
			}
			else if (subDirectory.find("0046") != string::npos)
			{
				upperAlphabetFound[5] = true;
			}
			else if (subDirectory.find("0047") != string::npos)
			{
				upperAlphabetFound[6] = true;
			}
			else if (subDirectory.find("0048") != string::npos)
			{
				upperAlphabetFound[7] = true;
			}
			else if (subDirectory.find("0049") != string::npos)
			{
				upperAlphabetFound[8] = true;
			}
			else if (subDirectory.find("004A") != string::npos)
			{
				upperAlphabetFound[9] = true;
			}
			else if (subDirectory.find("004B") != string::npos)
			{
				upperAlphabetFound[10] = true;
			}
			else if (subDirectory.find("004C") != string::npos)
			{
				upperAlphabetFound[11] = true;
			}
			else if (subDirectory.find("004D") != string::npos)
			{
				upperAlphabetFound[12] = true;
			}
			else if (subDirectory.find("004E") != string::npos)
			{
				upperAlphabetFound[13] = true;
			}
			else if (subDirectory.find("004F") != string::npos)
			{
				upperAlphabetFound[14] = true;
			}
			else if (subDirectory.find("0050") != string::npos)
			{
				upperAlphabetFound[15] = true;
			}
			else if (subDirectory.find("0051") != string::npos)
			{
				upperAlphabetFound[16] = true;
			}
			else if (subDirectory.find("0052") != string::npos)
			{
				upperAlphabetFound[17] = true;
			}
			else if (subDirectory.find("0053") != string::npos)
			{
				upperAlphabetFound[18] = true;
			}
			else if (subDirectory.find("0054") != string::npos)
			{
				upperAlphabetFound[19] = true;
			}
			else if (subDirectory.find("0055") != string::npos)
			{
				upperAlphabetFound[20] = true;
			}
			else if (subDirectory.find("0056") != string::npos)
			{
				upperAlphabetFound[21] = true;
			}
			else if (subDirectory.find("0057") != string::npos)
			{
				upperAlphabetFound[22] = true;
			}
			else if (subDirectory.find("0058") != string::npos)
			{
				upperAlphabetFound[23] = true;
			}
			else if (subDirectory.find("0059") != string::npos)
			{
				upperAlphabetFound[24] = true;
			}
			else if (subDirectory.find("005A") != string::npos)
			{
				upperAlphabetFound[25] = true;
			}

			#pragma endregion

			#pragma region CheckAllLowerCaseAlphabetCharactersExist

			else if (subDirectory.find("0061") != string::npos)
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
			if (!lowerAlphabetFound[i2] || !upperAlphabetFound[i2])
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

bool FontEngine::CreateFonts(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<string> fontFiles, int screenWidth, int screenHeight)
{
	try
	{
		for (int i = 0; i < fontFiles.size(); i++)
		{
			Font* font = new Font();

			vector<Character*> lowerCase = GetLowerCaseCharactersFromFontFolder(device, deviceContext, "fonts/" + fontFiles.at(i), screenWidth, screenHeight);

			font->_characters = lowerCase;

			_avaliableFonts.push_back(font);
		}

		return true;
	}
	catch (Exception& exception)
	{
		throw exception;
	}
}


vector<Character*> FontEngine::GetLowerCaseCharactersFromFontFolder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, string filePath, int screenWidth, int screenHeight)
{
	try
	{
		vector<Character*> lowerCaseFonts = vector<Character*>();

		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "a", "0061", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "b", "0062", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "c", "0063", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "d", "0064", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "e", "0065", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "f", "0066", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "g", "0067", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "h", "0068", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "i", "0069", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "j", "006A", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "k", "006B", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "l", "006C", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "m", "006D", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "n", "006E", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "o", "006F", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "p", "0070", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "q", "0071", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "r", "0072", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "s", "0073", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "t", "0074", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "u", "0075", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "v", "0076", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "w", "0077", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "y", "0078", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "x", "0079", screenWidth, screenHeight));
		lowerCaseFonts.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, "z", "007A", screenWidth, screenHeight));

		return lowerCaseFonts;
	}
	catch (Exception& exception)
	{
		throw Exception("Failed to create base characters for font.", exception);
	}
}

Character* FontEngine::CreateCharacterFromFontFolder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, string filePath, string name, string unicode, int screenWidth, int screenHeight)
{
	Bitmap* texture = new Bitmap;
	if (!texture) throw Exception("Failed to create the letter " + name + " for the font located at: " + filePath + ".");

	bool result = texture->Initialise(device, deviceContext, screenWidth, screenHeight, 64, 128, &(filePath + "/" + unicode + ".tga")[0u]);
	if (!result) throw Exception("Failed to initialise the texture for letter " + name + " for the character located at: " + filePath + "/" + unicode + ".tga");

	Character* character = new Character(name, unicode, texture);
}