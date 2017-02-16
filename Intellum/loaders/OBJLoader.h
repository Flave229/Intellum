#pragma once
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <Windows.h>	//Needed for xnamath.h
//#include <xnamath.h>	//For XMFLOAT3 and XMFLOAT2 definitions
#include <directxmath.h>
#include <fstream>		//For loading in an external file
#include <vector>		//For storing the XMFLOAT3/2 variables
#include <map>			//For fast searching when re-creating the index buffer
#include <d3d11.h>

#include "../common/Geometry.h"
#include "../common/Vertex.h"

using namespace DirectX;

namespace OBJLoader
{
	Geometry Load(char* filename, ID3D11Device* pd3dDevice, bool invertTexCoords = true);
	Geometry LoadFromBinaryFile(std::ifstream* binaryFile, ID3D11Device* pd3dDevice);
	Geometry LoadFromFile(char* filename, ID3D11Device* pd3dDevice, bool invertTexCoords = true);

	bool FindSimilarVertex(const Vertex& vertex, std::map<Vertex, unsigned short>& vertToIndexMap, unsigned short& index);

	void CreateIndices(const std::vector<XMFLOAT3>& inVertices, const std::vector<XMFLOAT2>& inTexCoords, const std::vector<XMFLOAT3>& inNormals, std::vector<unsigned short>& outIndices, std::vector<XMFLOAT3>& outVertices, std::vector<XMFLOAT2>& outTexCoords, std::vector<XMFLOAT3>& outNormals);
};