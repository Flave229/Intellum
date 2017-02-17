#pragma once
#include <map>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

class OBJGeometryData
{
public:
	map<string, vector<XMFLOAT3>> VertexData;
	map<string, vector<unsigned short>> IndexData;
};
