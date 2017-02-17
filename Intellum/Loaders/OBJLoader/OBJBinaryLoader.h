#pragma once
#include <string>
#include <fstream>

#include "IOBJLoader.h"

class OBJBinaryLoader : public IOBJLoader
{
public:
	OBJBinaryLoader();
	~OBJBinaryLoader();

	Geometry Load(char* filename, std::fstream* binaryFile, ID3D11Device* pd3dDevice, bool invertTexCoords) override;
};