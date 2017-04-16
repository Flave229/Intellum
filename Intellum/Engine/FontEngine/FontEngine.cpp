#include "FontEngine.h"

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

		result = FindFontsFolder();
		if (!result) return false;

		vector<string> potentialFonts = GetPotentialFonts();
		if (potentialFonts.size() <= 0) return false;

		vector<string> fonts = ValidatePotentialFonts(potentialFonts);
		if (fonts.size() <= 0) return false;

		result = CreateFonts(fonts, screenSize);
		if (!result) return false;

		return true;
	}
	catch (Exception& exception)
	{
		throw exception;
	}
}

vector<Entity*> FontEngine::ConvertTextToEntities(XMFLOAT2 position, string font, string input, XMFLOAT4 textColor, int fontSize)
{
	try
	{
		bool result = CheckFontExists(font);
		if (!result)
		{
			font = "Default";
		}

		vector<Character*> stringAsTexture = StringToCharacterTextureList(font, input);
		vector<Entity*> stringAsEntities;
		GeometryBuilder geometryBuilder = GeometryBuilder(_direct3D->GetDevice());

		for (int i = 0; i < stringAsTexture.size(); i++)
		{
			Entity* character = new Entity();

			AppearanceComponent* uiAppearance = new AppearanceComponent();
			uiAppearance->ShaderType = SHADER_UI;
			uiAppearance->Model = geometryBuilder.ForUI();
			uiAppearance->Textures = vector<Texture*> { stringAsTexture.at(i)->Texture };
			uiAppearance->Color = ColorShaderParameters(textColor);
			character->AddComponent(uiAppearance);

			TransformComponent* uiTransform = new TransformComponent();
			uiTransform->Position = XMFLOAT3(position.x + (fontSize * i), position.y, 0);
			character->AddComponent(uiTransform);

			UIComponent* uiComponent = new UIComponent();
			uiComponent->BitmapSize = XMFLOAT2(fontSize, fontSize * 2);
			character->AddComponent(uiComponent);

			stringAsEntities.push_back(character);
		}

		return stringAsEntities;
	}
	catch (Exception& exception)
	{
		throw Exception("The Font Engine failed to render the requested text: \"" + input + "\"", exception);
	}
	catch (...)
	{
		throw Exception("The Font Engine failed to render the requested text: \"" + input + "\"");
	}

	return vector<Entity*>();
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

bool FontEngine::CreateFonts(vector<string> fontFiles, Box screenSize)
{
	try
	{
		for (int i = 0; i < fontFiles.size(); i++)
		{
			Font* font = new Font();
			font->_fontName = fontFiles.at(i);

			vector<Character*> characters = GetCharactersFromFontFolder("fonts/" + fontFiles.at(i), screenSize);

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

vector<Character*> FontEngine::GetCharactersFromFontFolder(string filePath, Box screenSize)
{
	try
	{
		vector<Character*> characters = vector<Character*>();

		UnicodeDictionary unicodeDictionary = UnicodeDictionary();
		vector<string> unicodes = unicodeDictionary.GetSupportedUnicodeList();

		for(int i = 0; i < unicodes.size(); i++)
		{
			if (CheckCharacterExists(filePath + "/" + unicodes.at(i) + ".tga"))
				characters.push_back(CreateCharacterFromFontFolder(filePath, unicodeDictionary.GetCharacterForUnicode(unicodes.at(i)), unicodes.at(i), screenSize));
		}

		// Always reserve unicode 0000 in EVERY font to display the default error character in Fonts/Default
		characters.push_back(CreateCharacterFromFontFolder("Fonts/Default", unicodeDictionary.GetCharacterForUnicode("0000"), "0000", screenSize));

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

Character* FontEngine::CreateCharacterFromFontFolder(string filePath, string name, string unicode, Box screenSize)
{
	try
	{
		UIAppearance* uiAppearance = new UIAppearance(_direct3D, screenSize, Box(64, 128), vector<char*> { &(filePath + "/" + unicode + ".tga")[0u] }, "");
		Bitmap* texture = new Bitmap(_direct3D, _shader, uiAppearance);
		if (!texture) throw Exception("Failed to create the letter " + name + " for the font located at: " + filePath + ".");

		Character* character = new Character(name, unicode, texture);

		return character;
	}
	catch (Exception& exception)
	{
		throw Exception("Failed to create the letter " + name + " for the font located at: " + filePath + ".", exception);
	}
}

vector<Character*> FontEngine::StringToCharacterTextureList(string font, string input)
{
	try
	{
		Font* defaultFont = GetFont("Default");
		Font* chosenFont;
		if (font != "Default")
			chosenFont = GetFont(font);
		else
			chosenFont = defaultFont;

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