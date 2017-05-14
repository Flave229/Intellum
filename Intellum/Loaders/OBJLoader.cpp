#include "OBJLoader.h"

Geometry OBJLoader::Load(char* filename, ID3D11Device* pd3dDevice, bool invertTexCoords)
{
	std::string binaryFilename = filename;
	binaryFilename.append("Binary");
	std::fstream binaryInFile;
	binaryInFile.open(binaryFilename, std::ios::out | std::ios::in | std::ios::binary);

	IOBJLoader* objLoader;

	if (binaryInFile.good())
		objLoader = new OBJBinaryLoader();
	else
		objLoader = new OBJFileLoader();

	Geometry geometry = objLoader->Load(filename, &binaryInFile, pd3dDevice, invertTexCoords);
	
	delete objLoader;
	objLoader = nullptr;

	return geometry;
}