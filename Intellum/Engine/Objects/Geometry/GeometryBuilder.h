#pragma once
#include "../../DirectX3D.h"
#include "../../../loaders/OBJLoader.h"
#include "GridBuilder.h"

class GeometryBuilder
{
private:
	ID3D11Device* _device;
	GridBuilder _gridBuilder;

public:
	GeometryBuilder(ID3D11Device* device);
	~GeometryBuilder();

	Geometry FromFile(char* string) const;
	Geometry ForGrid(Box gridSize, XMFLOAT2 cellCount) const;
};
