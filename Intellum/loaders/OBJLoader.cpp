#include "OBJLoader.h"
#include <string>

bool OBJLoader::FindSimilarVertex(const Vertex& vertex, std::map<Vertex, unsigned short>& vertToIndexMap, unsigned short& index)
{
	auto it = vertToIndexMap.find(vertex);
	if(it == vertToIndexMap.end())
	{
		return false;
	}
	else
	{
		index = it->second;
		return true;
	}
}

void OBJLoader::CreateIndices(const std::vector<XMFLOAT3>& inVertices, const std::vector<XMFLOAT2>& inTexCoords, const std::vector<XMFLOAT3>& inNormals, std::vector<unsigned short>& outIndices, std::vector<XMFLOAT3>& outVertices, std::vector<XMFLOAT2>& outTexCoords, std::vector<XMFLOAT3>& outNormals)
{
	//Mapping from an already-existing Vertex to its corresponding index
	std::map<Vertex, unsigned short> vertToIndexMap;

	std::pair<Vertex, unsigned short> pair;

	unsigned long long numVertices = inVertices.size();
	for(int i = 0; i < numVertices; ++i)
	{
		Vertex vertex = { inVertices[i],  inTexCoords[i], inNormals[i] };

		unsigned short index;
		bool found = FindSimilarVertex(vertex, vertToIndexMap, index); //See if a vertex already exists in the buffer that has the same attributes as this one
		if(found) //if found, re-use it's index for the index buffer
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

Geometry OBJLoader::Load(char* filename, ID3D11Device* pd3dDevice, bool invertTexCoords)
{
	std::string binaryFilename = filename;
	binaryFilename.append("Binary");
	std::ifstream binaryInFile;
	binaryInFile.open(binaryFilename, std::ios::in | std::ios::binary);

	if(!binaryInFile.good())
		return LoadFromFile(filename, pd3dDevice, invertTexCoords);
	
	return LoadFromBinaryFile(&binaryInFile, pd3dDevice);
}

Geometry OBJLoader::LoadFromBinaryFile(std::ifstream* binaryFile, ID3D11Device* pd3dDevice)
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

	meshData.width = highestX - lowestX;
	meshData.height = highestY - lowestY;
	meshData.depth = highestZ - lowestZ;

	// This data has now been sent over to the GPU so we can delete this CPU-side stuff
	delete[] indices;
	delete[] finalVerts;

	return meshData;
}

Geometry OBJLoader::LoadFromFile(char* filename, ID3D11Device* pd3dDevice, bool invertTexCoords)
{
	std::ifstream inFile;
	inFile.open(filename);

	if (!inFile.good())
	{
		return Geometry();
	}
	else
	{
		//Vectors to store the vertex positions, normals and texture coordinates. Need to use vectors since they're resizeable and we have
		//no way of knowing ahead of time how large these meshes will be
		std::vector<XMFLOAT3> verts;
		std::vector<XMFLOAT3> normals;
		std::vector<XMFLOAT2> texCoords;

		//DirectX uses 1 index buffer, OBJ is optimized for storage and not rendering and so uses 3 smaller index buffers.....great...
		//We'll have to merge this into 1 index buffer which we'll do after loading in all of the required data.
		std::vector<unsigned short> vertIndices;
		std::vector<unsigned short> normalIndices;
		std::vector<unsigned short> textureIndices;

		std::string input;

		XMFLOAT3 vert;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		unsigned short vInd[3]; //indices for the vertex position
		unsigned short tInd[3]; //indices for the texture coordinate
		unsigned short nInd[3]; //indices for the normal
		std::string beforeFirstSlash;
		std::string afterFirstSlash;
		std::string afterSecondSlash;

		while (!inFile.eof()) //While we have yet to reach the end of the file...
		{
			inFile >> input; //Get the next input from the file

							 //Check what type of input it was, we are only interested in vertex positions, texture coordinates, normals and indices, nothing else
			if (input.compare("v") == 0) //Vertex position
			{
				inFile >> vert.x;
				inFile >> vert.y;
				inFile >> vert.z;

				verts.push_back(vert);
			}
			else if (input.compare("vt") == 0) //Texture coordinate
			{
				inFile >> texCoord.x;
				inFile >> texCoord.y;

				if (invertTexCoords) texCoord.y = 1.0f - texCoord.y;

				texCoords.push_back(texCoord);
			}
			else if (input.compare("vn") == 0) //Normal
			{
				inFile >> normal.x;
				inFile >> normal.y;
				inFile >> normal.z;

				normals.push_back(normal);
			}
			else if (input.compare("f") == 0) //Face
			{
				for (int i = 0; i < 3; ++i)
				{
					inFile >> input;
					unsigned long long slash = input.find("/"); //Find first forward slash
					unsigned long long secondSlash = input.find("/", slash + 1); //Find second forward slash

																				 //Extract from string
					beforeFirstSlash = input.substr(0, slash); //The vertex position index
					afterFirstSlash = input.substr(slash + 1, secondSlash - slash - 1); //The texture coordinate index
					afterSecondSlash = input.substr(secondSlash + 1); //The normal index

																	  //Parse into int
					vInd[i] = static_cast<unsigned short>(atoi(beforeFirstSlash.c_str())); //atoi = "ASCII to int"
					tInd[i] = static_cast<unsigned short>(atoi(afterFirstSlash.c_str()));
					nInd[i] = static_cast<unsigned short>(atoi(afterSecondSlash.c_str()));
				}

				//Place into vectors
				for (int i = 0; i < 3; ++i)
				{
					vertIndices.push_back(vInd[i] - 1);		//Minus 1 from each as these as OBJ indexes start from 1 whereas C++ arrays start from 0
					textureIndices.push_back(tInd[i] - 1);	//which is really annoying. Apart from Lua and SQL, there's not much else that has indexing 
					normalIndices.push_back(nInd[i] - 1);	//starting at 1. So many more languages index from 0, the .OBJ people screwed up there.
				}
			}
		}
		inFile.close(); //Finished with input file now, all the data we need has now been loaded in

						//Get vectors to be of same size, ready for singular indexing
		std::vector<XMFLOAT3> expandedVertices;
		std::vector<XMFLOAT3> expandedNormals;
		std::vector<XMFLOAT2> expandedTexCoords;
		unsigned long long numIndices = vertIndices.size();
		for (unsigned int i = 0; i < numIndices; i++)
		{
			expandedVertices.push_back(verts[vertIndices[i]]);
			expandedTexCoords.push_back(texCoords[textureIndices[i]]);
			expandedNormals.push_back(normals[normalIndices[i]]);
		}

		//Now to (finally) form the final vertex, texture coord, normal list and single index buffer using the above expanded vectors
		std::vector<unsigned short> meshIndices;
		meshIndices.reserve(numIndices);
		std::vector<XMFLOAT3> meshVertices;
		meshVertices.reserve(expandedVertices.size());
		std::vector<XMFLOAT3> meshNormals;
		meshNormals.reserve(expandedNormals.size());
		std::vector<XMFLOAT2> meshTexCoords;
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
		std::ofstream outbin(std::string(filename).append("Binary").c_str(), std::ios::out | std::ios::binary);
		outbin.write(reinterpret_cast<char*>(&numMeshVertices), sizeof(unsigned int));
		outbin.write(reinterpret_cast<char*>(&numMeshIndices), sizeof(unsigned int));
		outbin.write(reinterpret_cast<char*>(finalVerts), sizeof(Vertex) * numMeshVertices);
		outbin.write(reinterpret_cast<char*>(indicesArray), sizeof(unsigned short) * numMeshIndices);
		outbin.close();

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
}
