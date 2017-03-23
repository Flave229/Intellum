#include "GeometryBuilder.h"

GeometryBuilder::GeometryBuilder(ID3D11Device* device) : _device(device), _gridBuilder(GridBuilder(device))
{
}

GeometryBuilder::~GeometryBuilder()
{
}

Geometry GeometryBuilder::FromFile(char* string) const
{
	return OBJLoader::Load("data/models/sphere.obj", _device);
}

Geometry GeometryBuilder::ForGrid(Box gridSize, XMFLOAT2 cellCount) const
{
	return _gridBuilder.Build(gridSize, cellCount);
}