#pragma once
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <Windows.h>
#include <directxmath.h>
#include <fstream>
#include <vector>
#include <map>
#include <d3d11.h>
#include <string>

#include "OBJLoader/IOBJLoader.h"
#include "OBJLoader/OBJFileLoader.h"
#include "OBJLoader/OBJBinaryLoader.h"
#include "../common/Geometry.h"
#include "../common/Vertex.h"

using namespace DirectX;

namespace OBJLoader
{
	Geometry Load(char* filename, ID3D11Device* pd3dDevice, bool invertTexCoords = true);
};