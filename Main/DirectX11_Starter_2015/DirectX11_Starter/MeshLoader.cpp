#include "MeshLoader.h"

//std::string* split(std::string input) {
//	std::vector<std::string> strings;
//	
//}
using namespace DirectX;

MeshLoader::MeshLoader(const char * path)
{
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

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
				Vertex butts = { tempVert,red };
				vertList.push_back(butts);
			}
			if (strcmp(lineHeader, "f") == 0) {
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					return;
				}
				indicesList.push_back(vertexIndex[0]);
				indicesList.push_back(vertexIndex[1]);
				indicesList.push_back(vertexIndex[2]);

				vertArray = &vertList[0];
				indices = &indicesList[0];
			}
		}
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
int* MeshLoader::getIndices() {
	return indices;
}

MeshLoader::~MeshLoader()
{
}
