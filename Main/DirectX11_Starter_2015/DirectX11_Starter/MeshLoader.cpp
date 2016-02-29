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

				//tangent and binormal calc
				//algorithm from http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/

				XMVECTOR v0Pos = XMLoadFloat3(&vertList[vertexIndex[0]].Position);
				XMVECTOR v1Pos = XMLoadFloat3(&vertList[vertexIndex[1]].Position);
				XMVECTOR v2Pos = XMLoadFloat3(&vertList[vertexIndex[2]].Position);

				XMVECTOR uv0 = XMLoadFloat2(&vertList[vertexIndex[0]].UV);
				XMVECTOR uv1 = XMLoadFloat2(&vertList[vertexIndex[1]].UV);
				XMVECTOR uv2 = XMLoadFloat2(&vertList[vertexIndex[2]].UV);

				XMVECTOR deltaPos1 = v1Pos - v0Pos;
				XMVECTOR deltaPos2 = v2Pos - v0Pos;

				XMVECTOR deltaU1 = uv1 - uv0;
				XMVECTOR deltaU2 = uv2 - uv0;

				XMFLOAT3 dP1,dP2; 
				XMStoreFloat3(&dP1,deltaPos1);
				XMStoreFloat3(&dP2, deltaPos2);

				XMFLOAT2 dU1, dU2;
				XMStoreFloat2(&dU1, deltaU1);
				XMStoreFloat2(&dU2, deltaU2);

				float r = 1.0f / (dU1.x * dU2.y - dU1.y * dU2.x);
				XMVECTOR xmTangent = (deltaPos1 * dU2.y - deltaPos2 * dU1.y) * r;
				XMVECTOR xmBitangent = (deltaPos2 * dU1.x - deltaPos1 * dU2.x) * r;

				XMFLOAT3 tangent, bitangent;
				XMStoreFloat3(&tangent, xmTangent);
				XMStoreFloat3(&bitangent, xmBitangent);

				vertTan.push_back(tangent);
				vertTan.push_back(tangent);
				vertTan.push_back(tangent);

				vertBiTan.push_back(bitangent);
				vertBiTan.push_back(bitangent);
				vertBiTan.push_back(bitangent);
			}
		}

		avgTanBiTan();

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

		//this should work, but it doesn't :(
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

void MeshLoader::avgTanBiTan() {
	for (int i = 0; i < indicesList.size(); i+=3) {
		XMVECTOR xmTan = XMLoadFloat3(&vertTan[indicesList[i]]);
		xmTan += XMLoadFloat3(&vertTan[indicesList[i + 1]]);
		xmTan += XMLoadFloat3(&vertTan[indicesList[i + 2]]);

		xmTan /= 3;

		XMVECTOR xmBiTan = XMLoadFloat3(&vertBiTan[indicesList[i]]);
		xmBiTan += XMLoadFloat3(&vertBiTan[indicesList[i + 1]]);
		xmBiTan += XMLoadFloat3(&vertBiTan[indicesList[i + 2]]);

		xmBiTan /= 3;

		XMFLOAT3 tan, biTan;
		XMStoreFloat3(&tan, xmTan);
		XMStoreFloat3(&biTan, xmBiTan);

		/*vertTanActual.push_back(tan);
		vertBiTanActual.push_back(biTan);*/

		vertTan[indicesList[i + 0]] = tan;
		vertTan[indicesList[i + 1]] = tan;
		vertTan[indicesList[i + 2]] = tan;

		vertBiTan[indicesList[i + 0]] = biTan;
		vertBiTan[indicesList[i + 1]] = biTan;
		vertBiTan[indicesList[i + 2]] = biTan;
	}
	for (int i = 0; i < indicesList.size(); i++) {
		vertList[indicesList[i]].Tan = vertTan[indicesList[i]];
	}
	for (int i = 0; i < indicesList.size(); i++) {
		vertList[indicesList[i]].BiTan = vertBiTan[indicesList[i]];
	}
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
