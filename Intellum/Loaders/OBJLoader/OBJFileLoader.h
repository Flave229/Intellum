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
	static string Vertex() { return "v"; }
	static string Texture() { return "vt"; }
	static string Normal() { return "vn"; }
	static string Face() { return "f"; }
};

class OBJFileLoader : public IOBJLoader
{
private:
	static OBJGeometryData BuildGeometryDataFrom(char* fileName, bool invertTexCoords);
	static OBJGeometryData BuildExpandedGeometryDataFrom(OBJGeometryData geometryData);
	
	static OBJGeometryData CreateIndices(OBJGeometryData geometryData);
	static bool FindSimilarVertex(const Vertex& vertex, map<Vertex, unsigned short>& vertToIndexMap, unsigned short& index);

	static Vertex* BuildVertexObjectFrom(OBJGeometryData geometryData);

	static ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice, unsigned long long vertexCount, Vertex* finalVerts);
	static ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pd3dDevice, unsigned long long indicesCount, unsigned short* indicesArray);
public:
	OBJFileLoader();
	~OBJFileLoader() override = default;
		
	Geometry Load(char* filename, fstream* binaryFile, ID3D11Device* pd3dDevice, bool invertTexCoords) override;
};