#pragma once
#include <map>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

enum OBJDataType
{
	VERTICES,
	NORMALS,
	TEXTURECOORDINATES
};

class OBJGeometryData
{
public:
	map<OBJDataType, vector<XMFLOAT3>> VertexData;
	map<OBJDataType, vector<unsigned short>> IndexData;
};
