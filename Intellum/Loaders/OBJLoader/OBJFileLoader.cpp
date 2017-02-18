#include "OBJFileLoader.h"

OBJFileLoader::OBJFileLoader()
{
}

Geometry OBJFileLoader::Load(char* filename, fstream* binaryFile, ID3D11Device* pd3dDevice, bool invertTexCoords)
{
	try
	{
		string input;
		OBJGeometryData geometryData = BuildGeometryDataFrom(filename, invertTexCoords);
		geometryData = BuildExpandedGeometryDataFrom(geometryData);
		geometryData = CreateIndices(geometryData);

		Vertex* finalVerts = BuildVertexObjectFrom(geometryData);

		//Put data into vertex and index buffers, then pass the relevant data to the MeshData object.
		//The rest of the code will hopefully look familiar to you, as it's similar to whats in your InitVertexBuffer and InitIndexBuffer methods
		ID3D11Buffer* vertexBuffer;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * static_cast<UINT>(geometryData.VertexData[VERTICES].size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = finalVerts;

		pd3dDevice->CreateBuffer(&bd, &InitData, &vertexBuffer);

		Geometry meshData;
		meshData.VertexBuffer = vertexBuffer;
		meshData.VBOffset = 0;
		meshData.VBStride = sizeof(Vertex);

		unsigned short* indicesArray = new unsigned short[geometryData.IndexData[VERTICES].size()];
		unsigned long long numMeshIndices = geometryData.IndexData[VERTICES].size();
		for (unsigned int i = 0; i < numMeshIndices; ++i)
		{
			indicesArray[i] = geometryData.IndexData[VERTICES][i];
		}

		//Output data into binary file, the next time you run this function, the binary file will exist and will load that instead which is much quicker than parsing into vectors
		//std::ofstream outbin(std::string(filename).append("Binary").c_str(), std::ios::out | std::ios::binary);
		unsigned long long numMeshVertices = geometryData.VertexData[VERTICES].size();
		binaryFile->write(reinterpret_cast<char*>(&numMeshVertices), sizeof(unsigned int));
		binaryFile->write(reinterpret_cast<char*>(&numMeshIndices), sizeof(unsigned int));
		binaryFile->write(reinterpret_cast<char*>(finalVerts), sizeof(Vertex) * numMeshVertices);
		binaryFile->write(reinterpret_cast<char*>(indicesArray), sizeof(unsigned short) * numMeshIndices);
		binaryFile->close();

		ID3D11Buffer* indexBuffer;

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * static_cast<UINT>(geometryData.IndexData[VERTICES].size());
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indicesArray;
		pd3dDevice->CreateBuffer(&bd, &InitData, &indexBuffer);

		meshData.IndexCount = static_cast<UINT>(geometryData.IndexData[VERTICES].size());
		meshData.IndexBuffer = indexBuffer;

		//This data has now been sent over to the GPU so we can delete this CPU-side stuff
		delete[] indicesArray;
		delete[] finalVerts;

		return meshData;
	}
	catch (Exception& exception)
	{
		return Geometry();
	}
}

OBJGeometryData OBJFileLoader::BuildGeometryDataFrom(char* fileName, bool invertTexCoords)
{
	ifstream inFile;
	inFile.open(fileName);

	if (!inFile.good())
		throw Exception("Failed to load the file '" + string(fileName) + "'");

	OBJGeometryData geometryData;
	geometryData.VertexData = map<OBJDataType, vector<XMFLOAT3>>
	{
		{ VERTICES, vector<XMFLOAT3>() },
		{ NORMALS, vector<XMFLOAT3>() },
		{ TEXTURECOORDINATES, vector<XMFLOAT3>() }
	};
	geometryData.IndexData = map<OBJDataType, vector<unsigned short>>
	{
		{ VERTICES, vector<unsigned short>() },
		{ NORMALS, vector<unsigned short>() },
		{ TEXTURECOORDINATES, vector<unsigned short>() }
	};

	string input;

	while (!inFile.eof())
	{
		inFile >> input;

		if (input.compare(OBJFileType::Vertex()) == 0)
		{
			XMFLOAT3 vert;
			inFile >> vert.x;
			inFile >> vert.y;
			inFile >> vert.z;

			geometryData.VertexData[VERTICES].push_back(vert);
		}
		else if (input.compare(OBJFileType::Texture()) == 0)
		{
			XMFLOAT2 texCoord;
			inFile >> texCoord.x;
			inFile >> texCoord.y;

			if (invertTexCoords) 
				texCoord.y = 1.0f - texCoord.y;

			geometryData.VertexData[TEXTURECOORDINATES].push_back(XMFLOAT3(texCoord.x, texCoord.y, 0.0f));
		}
		else if (input.compare(OBJFileType::Normal()) == 0)
		{
			XMFLOAT3 normal;
			inFile >> normal.x;
			inFile >> normal.y;
			inFile >> normal.z;

			geometryData.VertexData[NORMALS].push_back(normal);
		}
		else if (input.compare(OBJFileType::Face()) == 0)
		{
			unsigned short vertexIndex[3];
			unsigned short textureIndex[3];
			unsigned short normalIndex[3];

			for (int i = 0; i < 3; ++i)
			{
				inFile >> input;
				unsigned long long slash = input.find("/");
				unsigned long long secondSlash = input.find("/", slash + 1);

				string beforeFirstSlash = input.substr(0, slash);
				string afterFirstSlash = input.substr(slash + 1, secondSlash - slash - 1);
				string afterSecondSlash = input.substr(secondSlash + 1);

				vertexIndex[i] = static_cast<unsigned short>(atoi(beforeFirstSlash.c_str()));
				textureIndex[i] = static_cast<unsigned short>(atoi(afterFirstSlash.c_str()));
				normalIndex[i] = static_cast<unsigned short>(atoi(afterSecondSlash.c_str()));
			}

			for (int i = 0; i < 3; ++i)
			{
				geometryData.IndexData[VERTICES].push_back(vertexIndex[i] - 1);
				geometryData.IndexData[TEXTURECOORDINATES].push_back(textureIndex[i] - 1);
				geometryData.IndexData[NORMALS].push_back(normalIndex[i] - 1);
			}
		}
	}

	inFile.close();
	return geometryData;
}

OBJGeometryData OBJFileLoader::BuildExpandedGeometryDataFrom(OBJGeometryData geometryData)
{
	OBJGeometryData expandedGeometryData;
	expandedGeometryData.VertexData = map<OBJDataType, vector<XMFLOAT3>>
	{
		{ VERTICES, vector<XMFLOAT3>() },
		{ NORMALS, vector<XMFLOAT3>() },
		{ TEXTURECOORDINATES, vector<XMFLOAT3>() }
	};
	expandedGeometryData.IndexData = map<OBJDataType, vector<unsigned short>>
	{
		{ VERTICES, geometryData.IndexData[VERTICES] }
	};

	for (unsigned int i = 0; i < geometryData.IndexData[VERTICES].size(); i++)
	{
		expandedGeometryData.VertexData[VERTICES].push_back(geometryData.VertexData[VERTICES][geometryData.IndexData[VERTICES][i]]);
		expandedGeometryData.VertexData[TEXTURECOORDINATES].push_back(geometryData.VertexData[TEXTURECOORDINATES][geometryData.IndexData[TEXTURECOORDINATES][i]]);
		expandedGeometryData.VertexData[NORMALS].push_back(geometryData.VertexData[NORMALS][geometryData.IndexData[NORMALS][i]]);
	}

	return expandedGeometryData;
}

OBJGeometryData OBJFileLoader::CreateIndices(OBJGeometryData geometryData)
{
	OBJGeometryData indexedGeometryData;
	indexedGeometryData.IndexData = map<OBJDataType, vector<unsigned short>>
	{
		{ VERTICES, vector<unsigned short>(geometryData.IndexData[VERTICES].size()) }
	};
	indexedGeometryData.VertexData = map<OBJDataType, vector<XMFLOAT3>>
	{
		{ VERTICES, vector<XMFLOAT3>(geometryData.VertexData[VERTICES].size()) },
		{ NORMALS, vector<XMFLOAT3>(geometryData.VertexData[NORMALS].size()) },
		{ TEXTURECOORDINATES, vector<XMFLOAT3>(geometryData.VertexData[TEXTURECOORDINATES].size()) }
	};

	map<Vertex, unsigned short> vertToIndexMap;

	for (int i = 0; i < geometryData.VertexData[VERTICES].size(); ++i)
	{
		XMFLOAT3 textureCoordinates = geometryData.VertexData[TEXTURECOORDINATES][i];
		Vertex vertex = { geometryData.VertexData[VERTICES][i], XMFLOAT2(textureCoordinates.x, textureCoordinates.y), geometryData.VertexData[NORMALS][i] };

		unsigned short index;
		if (FindSimilarVertex(vertex, vertToIndexMap, index))
		{
			indexedGeometryData.IndexData[VERTICES].push_back(index);
		}
		else
		{
			indexedGeometryData.VertexData[VERTICES].push_back(vertex.position);
			indexedGeometryData.VertexData[TEXTURECOORDINATES].push_back(XMFLOAT3(vertex.texture.x, vertex.texture.y, 0.0f));
			indexedGeometryData.VertexData[NORMALS].push_back(vertex.normal);

			unsigned short newIndex = static_cast<unsigned short>(indexedGeometryData.VertexData[VERTICES].size()) - 1;
			indexedGeometryData.IndexData[VERTICES].push_back(newIndex);

			vertToIndexMap.insert(pair<Vertex, unsigned short>(vertex, newIndex));
		}
	}

	return indexedGeometryData;
}

bool OBJFileLoader::FindSimilarVertex(const Vertex& vertex, map<Vertex, unsigned short>& vertToIndexMap, unsigned short& index)
{
	auto it = vertToIndexMap.find(vertex);
	if (it == vertToIndexMap.end())
	{
		return false;
	}
	else
	{
		index = it->second;
		return true;
	}
}

Vertex* OBJFileLoader::BuildVertexObjectFrom(OBJGeometryData geometryData)
{
	Vertex* finalVerts = new Vertex[geometryData.VertexData[VERTICES].size()];

	for (unsigned int i = 0; i < geometryData.VertexData[VERTICES].size(); i++)
	{
		finalVerts[i].position = geometryData.VertexData[VERTICES][i];
		finalVerts[i].normal = geometryData.VertexData[NORMALS][i];
		XMFLOAT3 textureCoordinates = geometryData.VertexData[TEXTURECOORDINATES][i];
		finalVerts[i].texture = XMFLOAT2(textureCoordinates.x, textureCoordinates.y);
	}

	return finalVerts;
}