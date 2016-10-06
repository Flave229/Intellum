#include "FontEngine.h"

FontEngine::FontEngine(IShaderType* shader) : _shader(shader)
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

bool FontEngine::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix,
	XMFLOAT3 cameraPosition, Light* light, int positionX, int positionY,
	string font, string input, XMFLOAT4 textColor)
{
	try
	{
		((FontShader*)_shader)->SetColorOverload(true, textColor);
		bool result = CheckFontExists(font);
		if (!result) return false;

		vector<Character*> stringAsTexture = StringToCharacterTextureList(font, input);

		for (int i = 0; i < stringAsTexture.size(); i++)
		{
			result = stringAsTexture.at(i)->_texture->Render(deviceContext, stringAsTexture.at(i)->_texture->GetIndexCount(), worldMatrix, viewMatrix,
				orthoMatrix, stringAsTexture.at(i)->_texture->GetTexture(), cameraPosition, light,
				positionX + (64 * i), positionY);
			if (!result) return false;
		}

		((FontShader*)_shader)->SetColorOverload(false);
		return true;
	}
	catch (Exception& exception)
	{
		throw Exception("The Font Engine failed to render the requested text: \"" + input + "\"", exception);
	}
	catch (...)
	{
		throw Exception("The Font Engine failed to render the requested text: \"" + input + "\"");
	}
}

Font* FontEngine::GetFont(string font)
{
	for (int i = 0; i < _avaliableFonts.size(); i++)
	{
		if (_avaliableFonts.at(i)->_fontName == font)
			return _avaliableFonts.at(i);
	}

	throw Exception("Could not find the font " + font);
}

bool FontEngine::CheckFontExists(string font)
{
	for (int i = 0; i < _avaliableFonts.size(); i++)
	{
		if (_avaliableFonts.at(i)->_fontName != font)
			continue;

		return true;
	}

	return false;
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
			font->_fontName = fontFiles.at(i);

			vector<Character*> characters = GetCharactersFromFontFolder(device, deviceContext, "fonts/" + fontFiles.at(i), screenWidth, screenHeight);

			font->_characters = characters;

			_avaliableFonts.push_back(font);
		}

		return true;
	}
	catch (Exception& exception)
	{
		throw exception;
	}
}

vector<Character*> FontEngine::GetCharactersFromFontFolder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, string filePath, int screenWidth, int screenHeight)
{
	try
	{
		vector<Character*> characters = vector<Character*>();

		UnicodeDictionary unicodeDictionary = UnicodeDictionary();
		vector<string> unicodes = unicodeDictionary.GetSupportedUnicodeList();

		for(int i = 0; i < unicodes.size(); i++)
		{
			if (CheckCharacterExists(filePath + "/" + unicodes.at(i) + ".tga"))
				characters.push_back(CreateCharacterFromFontFolder(device, deviceContext, filePath, unicodeDictionary.GetCharacterForUnicode(unicodes.at(i)), unicodes.at(i), screenWidth, screenHeight));
		}

		return characters;
	}
	catch (Exception& exception)
	{
		throw Exception("Failed to create characters for font.", exception);
	}
}

bool FontEngine::CheckCharacterExists(string filePath)
{
	FILE* file;

	bool error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0) return false;

	fclose(file);
	return true;
}

Character* FontEngine::CreateCharacterFromFontFolder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, string filePath, string name, string unicode, int screenWidth, int screenHeight)
{
	Bitmap* texture = new Bitmap(_shader);
	if (!texture) throw Exception("Failed to create the letter " + name + " for the font located at: " + filePath + ".");

	bool result = texture->Initialise(device, deviceContext, screenWidth, screenHeight, 64, 128, &(filePath + "/" + unicode + ".tga")[0u]);
	if (!result) throw Exception("Failed to initialise the texture for letter " + name + " for the character located at: " + filePath + "/" + unicode + ".tga");

	Character* character = new Character(name, unicode, texture);

	return character;
}

vector<Character*> FontEngine::StringToCharacterTextureList(string font, string input)
{
	try
	{
		Font* chosenFont = GetFont(font);
		vector<Character*> characterList;

		for (int i = 0; i < input.size(); i++)
		{
			characterList.push_back(chosenFont->GetCharacterByName(string(1, input.at(i))));
		}

		return characterList;
	}
	catch(Exception& exception)
	{
		throw Exception("The string \"" + input + "\" could not be converted into a texture for the font \"" + font + "\"", exception);
	}
	catch (...)
	{
		throw Exception("The string \"" + input + "\" could not be converted into a texture for the font \"" + font + "\"");
	}
}