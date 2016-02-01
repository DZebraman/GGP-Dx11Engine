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
private:
	Vertex* vertArray;
	unsigned int* indices;
	std::vector<Vertex> vertList;
	std::vector<unsigned int> indicesList;
	unsigned int numVerts;
	unsigned int numInd;
	//std::string*split(std::string input);
public:
	Vertex* getVerts();
	unsigned int* getIndices();
	unsigned int getNumVerts();
	unsigned int getNumInd();

	MeshLoader(const char * path);
	~MeshLoader();
};

