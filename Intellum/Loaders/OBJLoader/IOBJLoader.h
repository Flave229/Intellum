#pragma once
#include "../../Engine/Objects/Geometry/Geometry.h"
#include "../../common/Vertex.h"

class IOBJLoader
{
public:
	virtual ~IOBJLoader() {}
	virtual Geometry Load(char* filename, std::fstream* binaryFile, ID3D11Device* pd3dDevice, bool invertTexCoords) = 0;
};
