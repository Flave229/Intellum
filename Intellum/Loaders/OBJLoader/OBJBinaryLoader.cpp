#include "OBJBinaryLoader.h"


OBJBinaryLoader::OBJBinaryLoader()
{
}


OBJBinaryLoader::~OBJBinaryLoader()
{
}

Geometry OBJBinaryLoader::Load(char* filename, std::fstream* binaryFile, ID3D11Device* pd3dDevice, bool invertTexCoords)
{
	Geometry meshData;

	//Read in array sizes
	binaryFile->read(reinterpret_cast<char*>(&meshData.VertexCount), sizeof(meshData.VertexCount));
	binaryFile->read(reinterpret_cast<char*>(&meshData.IndexCount), sizeof(meshData.IndexCount));

	//Read in data from binary file
	Vertex* finalVerts = new Vertex[meshData.VertexCount];
	unsigned long* indices = new unsigned long[meshData.IndexCount];
	binaryFile->read(reinterpret_cast<char*>(finalVerts), sizeof(Vertex) * meshData.VertexCount);
	binaryFile->read(reinterpret_cast<char*>(indices), sizeof(unsigned long) * meshData.IndexCount);

	// Setup description for the Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * meshData.VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = finalVerts;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	pd3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &meshData.VertexBuffer);

	meshData.VBOffset = 0;
	meshData.VBStride = sizeof(Vertex);

	// Setup description for the Index Buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * meshData.IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	pd3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &meshData.IndexBuffer);

	float lowestX = finalVerts[0].position.x;
	float highestX = finalVerts[0].position.x;
	float lowestY = finalVerts[0].position.y;
	float highestY = finalVerts[0].position.y;
	float lowestZ = finalVerts[0].position.z;
	float highestZ = finalVerts[0].position.z;

	for (int i = 0; i < static_cast<int>(meshData.VertexCount); i++) {
		if (finalVerts[i].position.x < lowestX) {
			lowestX = finalVerts[i].position.x;
		}
		else if (finalVerts[i].position.x > highestX) {
			highestX = finalVerts[i].position.x;
		}

		if (finalVerts[i].position.y < lowestY) {
			lowestY = finalVerts[i].position.y;
		}
		else if (finalVerts[i].position.y > highestY) {
			highestY = finalVerts[i].position.y;
		}

		if (finalVerts[i].position.z < lowestZ) {
			lowestZ = finalVerts[i].position.z;
		}
		else if (finalVerts[i].position.z > highestZ) {
			highestZ = finalVerts[i].position.z;
		}
	}

	meshData.Size = XMFLOAT3(highestX - lowestX, highestY - lowestY, highestZ - lowestZ);
	
	delete[] indices;
	delete[] finalVerts;

	return meshData;
}