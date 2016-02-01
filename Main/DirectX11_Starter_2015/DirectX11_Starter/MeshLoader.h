#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <DirectXMath.h>
#include "Vertex.h"
#include <vector>
class MeshLoader
{
	Vertex* vertArray;
	int* indices;
	std::vector<Vertex> vertList;
	std::vector<int> indicesList;
	//std::string*split(std::string input);
public:
	Vertex* getVerts();
	int* getIndices();
	MeshLoader(const char * path);
	~MeshLoader();
};

