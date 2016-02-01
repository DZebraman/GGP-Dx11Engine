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
	//std::string*split(std::string input);
public:
	MeshLoader(std::string fileName);
	~MeshLoader();
};

