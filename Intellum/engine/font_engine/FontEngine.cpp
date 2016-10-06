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
		
		validatedFonts.push_back(potentialFonts.at(i));
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