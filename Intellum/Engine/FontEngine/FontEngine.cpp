﻿#include "FontEngine.h"
#include "FontRetriever.h"

FontEngine::FontEngine(DirectX3D* direct3D, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IShaderType* shader) : _direct3D(direct3D), _device(device), _deviceContext(deviceContext), _shader(shader)
{
}

FontEngine::~FontEngine()
{
}

bool FontEngine::SearchForAvaliableFonts(Box screenSize)
{
	try
	{
		bool result;

		FontRetriever fontRetriever = FontRetriever();
		vector<string> fonts = FontRetriever::RetrieveAvaliableFontNames();;

		result = CreateFonts(fonts, screenSize);
		if (!result) return false;

		return true;
	}
	catch (Exception& exception)
	{
		throw exception;
	}
}

vector<TextTexture> FontEngine::ConvertTextToTextEntity(XMFLOAT2 position, string font, string input, XMFLOAT4 textColor, int fontSize)
{
	try
	{
		bool result = CheckFontExists(font);
		if (!result)
		{
			font = "Default";
		}

		vector<TextTexture> textTextures = vector<TextTexture>();
		vector<Texture*> stringAsTexture = StringToCharacterTextureList(font, input);
		GeometryBuilder geometryBuilder = GeometryBuilder(_direct3D->GetDevice());

		for (int i = 0; i < stringAsTexture.size(); i++)
		{
			TextTexture character = TextTexture(stringAsTexture.at(i));
			character.Color = ColorShaderParameters(textColor);
			character.Model = geometryBuilder.ForUI();
			character.Position = XMFLOAT2(position.x + (fontSize * i), position.y);
			character.Size = fontSize;

			textTextures.push_back(character);
		}

		return textTextures;
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

Texture* FontEngine::ConvertCharacterToTexture(string font, string character)
{
	try
	{
		bool result = CheckFontExists(font);
		if (!result)
		{
			font = "Default";
		}

		return StringToCharacterTextureList(font, character).at(0);
	}
	catch (Exception& exception)
	{
		throw Exception("The Font Engine failed to render the requested text: \"" + character + "\"", exception);
	}
	catch (...)
	{
		throw Exception("The Font Engine failed to render the requested text: \"" + character + "\"");
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

bool FontEngine::CreateFonts(vector<string> fontFiles, Box screenSize)
{
	try
	{
		for (int i = 0; i < fontFiles.size(); i++)
		{
			Font* font = new Font();
			font->_fontName = fontFiles.at(i);

			vector<Character> characters = GetCharactersFromFontFolder("Content/Fonts/" + fontFiles.at(i), screenSize);

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

vector<Character> FontEngine::GetCharactersFromFontFolder(string filePath, Box screenSize)
{
	try
	{
		vector<Character> characters = vector<Character>();

		UnicodeDictionary unicodeDictionary = UnicodeDictionary();
		vector<string> unicodes = unicodeDictionary.GetSupportedUnicodeList();

		for(int i = 0; i < unicodes.size(); i++)
		{
			if (CheckCharacterExists(filePath + "/" + unicodes.at(i) + ".tga"))
				characters.push_back(CreateCharacterFromFontFolder(filePath, unicodeDictionary.GetCharacterForUnicode(unicodes.at(i)), unicodes.at(i), screenSize));
		}

		// Always reserve unicode 0000 in EVERY font to display the default error character in Fonts/Default
		characters.push_back(CreateCharacterFromFontFolder("Content/Fonts/Default", unicodeDictionary.GetCharacterForUnicode("0000"), "0000", screenSize));

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

	int error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0) return false;

	fclose(file);
	return true;
}

Character FontEngine::CreateCharacterFromFontFolder(string filePath, string name, string unicode, Box screenSize)
{
	try
	{
		Texture* texture = CreateTexture::From(_direct3D, &(filePath + "/" + unicode + ".tga")[0u]);
		if (!texture) throw Exception("Failed to create the letter " + name + " for the font located at: " + filePath + ".");

		Character character = Character(name, unicode, texture);

		return character;
	}
	catch (Exception& exception)
	{
		throw Exception("Failed to create the letter " + name + " for the font located at: " + filePath + ".", exception);
	}
}

vector<Texture*> FontEngine::StringToCharacterTextureList(string font, string input)
{
	try
	{
		Font* chosenFont;
		if (font != "Default")
			chosenFont = GetFont(font);
		else
			chosenFont = GetFont("Default");

		vector<Texture*> characterList;

		for (int i = 0; i < input.size(); i++)
		{
			characterList.push_back(chosenFont->GetCharacterTextureByName(string(1, input.at(i))));
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