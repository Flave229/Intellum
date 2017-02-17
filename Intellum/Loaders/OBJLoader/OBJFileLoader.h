#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "IOBJLoader.h"
#include "../models/OBJGeometryData.h"

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
	OBJGeometryData ConstructGeometryDataFrom(std::ifstream* inFile, bool invertTexCoords);
	
	static void CreateIndices(const std::vector<XMFLOAT3>& inVertices, const std::vector<XMFLOAT2>& inTexCoords, const std::vector<XMFLOAT3>& inNormals, std::vector<unsigned short>& outIndices, std::vector<XMFLOAT3>& outVertices, std::vector<XMFLOAT2>& outTexCoords, std::vector<XMFLOAT3>& outNormals);
	static bool FindSimilarVertex(const Vertex& vertex, std::map<Vertex, unsigned short>& vertToIndexMap, unsigned short& index);
public:
	OBJFileLoader();
	~OBJFileLoader() override = default;

	Geometry Load(char* filename, std::fstream* binaryFile, ID3D11Device* pd3dDevice, bool invertTexCoords) override;
};