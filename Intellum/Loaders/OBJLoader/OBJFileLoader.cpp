#include "OBJFileLoader.h"

OBJFileLoader::OBJFileLoader()
{
}

Geometry OBJFileLoader::Load(char* filename, fstream* binaryFile, ID3D11Device* pd3dDevice, bool invertTexCoords)
{
	ifstream inFile;
	inFile.open(filename);

	if (!inFile.good())
	{
		return Geometry();
	}

	string input;

	XMFLOAT3 vert;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	unsigned short vInd[3]; //indices for the vertex position
	unsigned short tInd[3]; //indices for the texture coordinate
	unsigned short nInd[3]; //indices for the normal
	string beforeFirstSlash;
	string afterFirstSlash;
	string afterSecondSlash;

	OBJGeometryData geometryData = ConstructGeometryDataFrom(&inFile, invertTexCoords);

					//Get vectors to be of same size, ready for singular indexing
	vector<XMFLOAT3> expandedVertices;
	vector<XMFLOAT3> expandedNormals;
	vector<XMFLOAT2> expandedTexCoords;
	unsigned long long numIndices = geometryData.IndexData["Vertices"].size();
	for (unsigned int i = 0; i < numIndices; i++)
	{
		expandedVertices.push_back(geometryData.VertexData["Vertices"][geometryData.IndexData["Vertices"][i]]);
		XMFLOAT3 textureCoordinates = geometryData.VertexData["TextureCoordinates"][geometryData.IndexData["TextureCoordinates"][i]];
		expandedTexCoords.push_back(XMFLOAT2(textureCoordinates.x, textureCoordinates.y));
		expandedNormals.push_back(geometryData.VertexData["Normals"][geometryData.IndexData["Normals"][i]]);
	}

	//Now to (finally) form the final vertex, texture coord, normal list and single index buffer using the above expanded vectors
	vector<unsigned short> meshIndices;
	meshIndices.reserve(numIndices);
	vector<XMFLOAT3> meshVertices;
	meshVertices.reserve(expandedVertices.size());
	vector<XMFLOAT3> meshNormals;
	meshNormals.reserve(expandedNormals.size());
	vector<XMFLOAT2> meshTexCoords;
	meshTexCoords.reserve(expandedTexCoords.size());

	CreateIndices(expandedVertices, expandedTexCoords, expandedNormals, meshIndices, meshVertices, meshTexCoords, meshNormals);

	Geometry meshData;

	//Turn data from vector form to arrays
	Vertex* finalVerts = new Vertex[meshVertices.size()];
	unsigned long long numMeshVertices = meshVertices.size();
	for (unsigned int i = 0; i < numMeshVertices; ++i)
	{
		finalVerts[i].position = meshVertices[i];
		finalVerts[i].normal = meshNormals[i];
		finalVerts[i].texture = meshTexCoords[i];
	}

	//Put data into vertex and index buffers, then pass the relevant data to the MeshData object.
	//The rest of the code will hopefully look familiar to you, as it's similar to whats in your InitVertexBuffer and InitIndexBuffer methods
	ID3D11Buffer* vertexBuffer;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * static_cast<UINT>(meshVertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = finalVerts;

	pd3dDevice->CreateBuffer(&bd, &InitData, &vertexBuffer);

	meshData.VertexBuffer = vertexBuffer;
	meshData.VBOffset = 0;
	meshData.VBStride = sizeof(Vertex);

	unsigned short* indicesArray = new unsigned short[meshIndices.size()];
	unsigned long long numMeshIndices = meshIndices.size();
	for (unsigned int i = 0; i < numMeshIndices; ++i)
	{
		indicesArray[i] = meshIndices[i];
	}

	//Output data into binary file, the next time you run this function, the binary file will exist and will load that instead which is much quicker than parsing into vectors
	//std::ofstream outbin(std::string(filename).append("Binary").c_str(), std::ios::out | std::ios::binary);
	binaryFile->write(reinterpret_cast<char*>(&numMeshVertices), sizeof(unsigned int));
	binaryFile->write(reinterpret_cast<char*>(&numMeshIndices), sizeof(unsigned int));
	binaryFile->write(reinterpret_cast<char*>(finalVerts), sizeof(Vertex) * numMeshVertices);
	binaryFile->write(reinterpret_cast<char*>(indicesArray), sizeof(unsigned short) * numMeshIndices);
	binaryFile->close();

	ID3D11Buffer* indexBuffer;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * static_cast<UINT>(meshIndices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indicesArray;
	pd3dDevice->CreateBuffer(&bd, &InitData, &indexBuffer);

	meshData.IndexCount = static_cast<UINT>(meshIndices.size());
	meshData.IndexBuffer = indexBuffer;

	//This data has now been sent over to the GPU so we can delete this CPU-side stuff
	delete[] indicesArray;
	delete[] finalVerts;

	return meshData;
}

OBJGeometryData OBJFileLoader::ConstructGeometryDataFrom(ifstream* inFile, bool invertTexCoords)
{
	OBJGeometryData geometryData;
	geometryData.VertexData = map<string, vector<XMFLOAT3>>
	{
		{"Vertices", vector<XMFLOAT3>()},
		{"Normals", vector<XMFLOAT3>()},
		{"TextureCoordinates", vector<XMFLOAT3>()}
	};
	geometryData.IndexData = map<string, vector<unsigned short>>
	{
		{ "Vertices", vector<unsigned short>() },
		{ "Normals", vector<unsigned short>() },
		{ "TextureCoordinates", vector<unsigned short>() }
	};

	string input;
	XMFLOAT3 vert;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	unsigned short vertexIndex[3];
	unsigned short textureIndex[3];
	unsigned short normalIndex[3];

	while (!inFile->eof())
	{
		*inFile >> input;

		if (input.compare(OBJFileType::Vertex()) == 0)
		{
			*inFile >> vert.x;
			*inFile >> vert.y;
			*inFile >> vert.z;

			geometryData.VertexData["Vertices"].push_back(vert);
		}
		else if (input.compare(OBJFileType::Texture()) == 0)
		{
			*inFile >> texCoord.x;
			*inFile >> texCoord.y;

			if (invertTexCoords) texCoord.y = 1.0f - texCoord.y;

			geometryData.VertexData["TextureCoordinates"].push_back(XMFLOAT3(texCoord.x, texCoord.y, 0.0f));
		}
		else if (input.compare(OBJFileType::Normal()) == 0)
		{
			*inFile >> normal.x;
			*inFile >> normal.y;
			*inFile >> normal.z;

			geometryData.VertexData["Normals"].push_back(normal);
		}
		else if (input.compare(OBJFileType::Face()) == 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				*inFile >> input;
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
				geometryData.IndexData["Vertices"].push_back(vertexIndex[i] - 1);
				geometryData.IndexData["TextureCoordinates"].push_back(textureIndex[i] - 1);
				geometryData.IndexData["Normals"].push_back(normalIndex[i] - 1);
			}
		}
	}

	inFile->close();

	return geometryData;
}

void OBJFileLoader::CreateIndices(const vector<XMFLOAT3>& inVertices, const vector<XMFLOAT2>& inTexCoords, const vector<XMFLOAT3>& inNormals, vector<unsigned short>& outIndices, vector<XMFLOAT3>& outVertices, vector<XMFLOAT2>& outTexCoords, vector<XMFLOAT3>& outNormals)
{
	//Mapping from an already-existing Vertex to its corresponding index
	map<Vertex, unsigned short> vertToIndexMap;

	pair<Vertex, unsigned short> pair;

	unsigned long long numVertices = inVertices.size();
	for (int i = 0; i < numVertices; ++i)
	{
		Vertex vertex = { inVertices[i],  inTexCoords[i], inNormals[i] };

		unsigned short index;
		bool found = FindSimilarVertex(vertex, vertToIndexMap, index); //See if a vertex already exists in the buffer that has the same attributes as this one
		if (found) //if found, re-use it's index for the index buffer
		{
			outIndices.push_back(index);
		}
		else //if not found, add it to the buffer
		{
			outVertices.push_back(vertex.position);
			outTexCoords.push_back(vertex.texture);
			outNormals.push_back(vertex.normal);

			unsigned short newIndex = static_cast<unsigned short>(outVertices.size()) - 1;
			outIndices.push_back(newIndex);

			//Add it to the map
			pair.first = vertex;
			pair.second = newIndex;
			vertToIndexMap.insert(pair);
		}
	}
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