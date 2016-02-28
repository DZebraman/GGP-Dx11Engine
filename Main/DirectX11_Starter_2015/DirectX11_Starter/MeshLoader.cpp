#include "MeshLoader.h"
#pragma warning(disable:4996)
//std::string* split(std::string input) {
//	std::vector<std::string> strings;
//	
//}
using namespace DirectX;

MeshLoader::MeshLoader(const char * path)
{
	numVerts = 0;
	numInd = 0;



	FILE * file = fopen(path, "r");
	if (file != NULL) {
		while (1) {
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;
			if (strcmp(lineHeader, "v") == 0) {
				XMFLOAT3 tempVert;
				fscanf(file, "%f %f %f\n", &tempVert.x, &tempVert.y, &tempVert.z);
				Vertex butts = { tempVert, XMFLOAT3(0,0,-1),XMFLOAT2(0,0) };
				vertPos.push_back(tempVert);
				vertList.push_back(butts);
			}
			if (strcmp(lineHeader, "vn") == 0) {
				XMFLOAT3 tempNRM;
				fscanf(file, "%f %f %f\n", &tempNRM.x, &tempNRM.y, &tempNRM.z);
				vertNrm.push_back(tempNRM);
			}
			if (strcmp(lineHeader, "vt") == 0) {
				XMFLOAT2 tempUV;
				fscanf(file, "%f %f\n", &tempUV.x, &tempUV.y);
				vertUV.push_back(tempUV);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					return;
				}
				vertexIndex[0] -= 1;
				vertexIndex[1] -= 1;
				vertexIndex[2] -= 1;

				uvIndex[0] -= 1;
				uvIndex[1] -= 1;
				uvIndex[2] -= 1;

				normalIndex[0] -= 1;
				normalIndex[1] -= 1;
				normalIndex[2] -= 1;

				indicesList.push_back(vertexIndex[0]);
				indicesList.push_back(vertexIndex[1]);
				indicesList.push_back(vertexIndex[2]);

				vertList[vertexIndex[0]].Normal = vertNrm[normalIndex[0]];
				vertList[vertexIndex[1]].Normal = vertNrm[normalIndex[1]];
				vertList[vertexIndex[2]].Normal = vertNrm[normalIndex[2]];

				vertList[vertexIndex[0]].UV = vertUV[uvIndex[0]];
				vertList[vertexIndex[0]].UV.y = 1-vertUV[uvIndex[0]].y;

				vertList[vertexIndex[1]].UV = vertUV[uvIndex[1]];
				vertList[vertexIndex[1]].UV.y = 1 - vertUV[uvIndex[1]].y;

				vertList[vertexIndex[2]].UV = vertUV[uvIndex[2]];
				vertList[vertexIndex[2]].UV.y = 1 - vertUV[uvIndex[2]].y;
			}
		}

		numVerts = vertPos.size();
		numInd = indicesList.size();

		vertArray = new Vertex[numVerts];
		for (int i = 0; i < numVerts; i++) {
			vertArray[i] = vertList[i];
		}

		indices = new unsigned int[numInd];
		for (int i = 0; i < numInd; i++) {
			indices[i] = indicesList[i];
		}

		/*vertArray = &vertList[0];
		indices = &indicesList[0];*/
	}

	/*std::ifstream file(fileName);
	std::string line;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			if (line[0] == 'v' && line[1] != 'n' && line[1] != 't') {
				XMFLOAT3 tempVec;
				fscanf(file)
			}else if (line[0] == 'v' && line[1] == 'n') {

			}else if (line[0] == 'v' && line[1] == 't') {

			}else if (line[0] == 'f') {

			}
		}
	}*/
}

Vertex* MeshLoader::getVerts() {
	return vertArray;
}
unsigned int* MeshLoader::getIndices() {
	return indices;
}

unsigned int MeshLoader::getNumVerts() {
	return numVerts;
}

unsigned int MeshLoader::getNumInd() {
	return numInd;
}

MeshLoader::~MeshLoader()
{
	delete indices;
	delete vertArray;
}
