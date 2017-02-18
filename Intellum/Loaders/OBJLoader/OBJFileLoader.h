#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "IOBJLoader.h"
#include "../models/OBJGeometryData.h"
#include "../../ErrorHandling/Exception.h"

class OBJFileType
{
private:
	OBJFileType() {}
	~OBJFileType() {}

public:
	static std::string Vertex() { return "v"; }
	static std::string Texture() { return "vt"; }
	static std::string Normal() { return "vn"; }
	static std::string Face() { return "f"; }
};

class OBJFileLoader : public IOBJLoader
{
private:
	static OBJGeometryData BuildGeometryDataFrom(char* fileName, bool invertTexCoords);
	static OBJGeometryData BuildExpandedGeometryDataFrom(OBJGeometryData geometryData);
	
	static OBJGeometryData CreateIndices(OBJGeometryData geometryData);
	static bool FindSimilarVertex(const Vertex& vertex, std::map<Vertex, unsigned short>& vertToIndexMap, unsigned short& index);

	Vertex* BuildVertexObjectFrom(OBJGeometryData geometryData);
public:
	OBJFileLoader();
	~OBJFileLoader() override = default;

	Geometry Load(char* filename, std::fstream* binaryFile, ID3D11Device* pd3dDevice, bool invertTexCoords) override;
};