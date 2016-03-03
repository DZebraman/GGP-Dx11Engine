#include "MeshLoader.h"
#pragma warning(disable:4996)
//std::string* split(std::string input) {
//	std::vector<std::string> strings;
//	
//}
using namespace DirectX;

MeshLoader::MeshLoader(const char * path)
{
//	numVerts = 0;
//	numInd = 0;
//
//
//
//	FILE * file = fopen(path, "r");
//	if (file != NULL) {
//		while (1) {
//			char lineHeader[128];
//			int res = fscanf(file, "%s", lineHeader);
//			if (res == EOF)
//				break;
//			if (strcmp(lineHeader, "v") == 0) {
//				XMFLOAT3 tempVert;
//				fscanf(file, "%f %f %f\n", &tempVert.x, &tempVert.y, &tempVert.z);
//				Vertex butts = { tempVert, XMFLOAT3(0,0,-1),XMFLOAT2(0,0) };
//				vertPos.push_back(tempVert);
//				vertList.push_back(butts);
//			}
//			if (strcmp(lineHeader, "vn") == 0) {
//				XMFLOAT3 tempNRM;
//				fscanf(file, "%f %f %f\n", &tempNRM.x, &tempNRM.y, &tempNRM.z);
//				vertNrm.push_back(tempNRM);
//			}
//			if (strcmp(lineHeader, "vt") == 0) {
//				XMFLOAT2 tempUV;
//				fscanf(file, "%f %f\n", &tempUV.x, &tempUV.y);
//				vertUV.push_back(tempUV);
//			}
//			else if (strcmp(lineHeader, "f") == 0) {
//				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//				if (matches != 9) {
//					return;
//				}
//				vertexIndex[0] -= 1;
//				vertexIndex[1] -= 1;
//				vertexIndex[2] -= 1;
//
//				uvIndex[0] -= 1;
//				uvIndex[1] -= 1;
//				uvIndex[2] -= 1;
//
//				normalIndex[0] -= 1;
//				normalIndex[1] -= 1;
//				normalIndex[2] -= 1;
//
//				indicesList.push_back(vertexIndex[0]);
//				indicesList.push_back(vertexIndex[1]);
//				indicesList.push_back(vertexIndex[2]);
//
//				vertList[vertexIndex[0]].Normal = vertNrm[normalIndex[0]];
//				vertList[vertexIndex[1]].Normal = vertNrm[normalIndex[1]];
//				vertList[vertexIndex[2]].Normal = vertNrm[normalIndex[2]];
//
//				vertList[vertexIndex[0]].UV = vertUV[uvIndex[0]];
//				vertList[vertexIndex[0]].UV.y = 1-vertUV[uvIndex[0]].y;
//
//				vertList[vertexIndex[1]].UV = vertUV[uvIndex[1]];
//				vertList[vertexIndex[1]].UV.y = 1 - vertUV[uvIndex[1]].y;
//
//				vertList[vertexIndex[2]].UV = vertUV[uvIndex[2]];
//				vertList[vertexIndex[2]].UV.y = 1 - vertUV[uvIndex[2]].y;
//
//				//tangent and binormal calc
//				//algorithm from http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
//
//				XMVECTOR v0Pos = XMLoadFloat3(&vertList[vertexIndex[0]].Position);
//				XMVECTOR v1Pos = XMLoadFloat3(&vertList[vertexIndex[1]].Position);
//				XMVECTOR v2Pos = XMLoadFloat3(&vertList[vertexIndex[2]].Position);
//
//				XMVECTOR uv0 = XMLoadFloat2(&vertList[vertexIndex[0]].UV);
//				XMVECTOR uv1 = XMLoadFloat2(&vertList[vertexIndex[1]].UV);
//				XMVECTOR uv2 = XMLoadFloat2(&vertList[vertexIndex[2]].UV);
//
//				XMVECTOR deltaPos1 = v1Pos - v0Pos;
//				XMVECTOR deltaPos2 = v2Pos - v0Pos;
//
//				XMVECTOR deltaU1 = uv1 - uv0;
//				XMVECTOR deltaU2 = uv2 - uv0;
//
//				XMFLOAT3 dP1,dP2; 
//				XMStoreFloat3(&dP1, deltaPos1);
//				XMStoreFloat3(&dP2, deltaPos2);
//
//				XMFLOAT2 dU1, dU2;
//				XMStoreFloat2(&dU1, deltaU1);
//				XMStoreFloat2(&dU2, deltaU2);
//
//				float r = 1.0f / (dU1.x * dU2.y - dU1.y * dU2.x);
//				XMVECTOR xmTangent = (deltaPos1 * dU2.y - deltaPos2 * dU1.y) * r;
//				XMVECTOR xmBitangent = (deltaPos2 * dU1.x - deltaPos1 * dU2.x) * r;
//
//				XMFLOAT3 tangent, bitangent;
//				XMStoreFloat3(&tangent, xmTangent);
//				XMStoreFloat3(&bitangent, xmBitangent);
//
//				vertTan.push_back(tangent);
//				vertTan.push_back(tangent);
//				vertTan.push_back(tangent);
//
//				vertBiTan.push_back(bitangent);
//				vertBiTan.push_back(bitangent);
//				vertBiTan.push_back(bitangent);
//			}
//		}
//
//		avgTanBiTan();
//
//		numVerts = vertPos.size();
//		numInd = indicesList.size();
//
//		vertArray = new Vertex[numVerts];
//		for (int i = 0; i < numVerts; i++) {
//			vertArray[i] = vertList[i];
//		}
//
//		indices = new unsigned int[numInd];
//		for (int i = 0; i < numInd; i++) {
//			indices[i] = indicesList[i];
//		}
//
//		//this should work, but it doesn't :(
//		/*vertArray = &vertList[0];
//		indices = &indicesList[0];*/
//	}
//
//	/*std::ifstream file(fileName);
//	std::string line;
//	if (file.is_open()) {
//		while (std::getline(file, line)) {
//			if (line[0] == 'v' && line[1] != 'n' && line[1] != 't') {
//				XMFLOAT3 tempVec;
//				fscanf(file)
//			}else if (line[0] == 'v' && line[1] == 'n') {
//
//			}else if (line[0] == 'v' && line[1] == 't') {
//
//			}else if (line[0] == 'f') {
//
//			}
//		}
//	}*/
	// File input object
	std::ifstream obj(path); // <-- Replace filename with your parameter
	
							 // Check for successful open
	if (!obj.is_open())
	return;
	
	// Variables used while reading the file
	
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading
	
										 // Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);
	
		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);
	
			// Add to the list of normals
			vertNrm.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);
	
			// Add to the list of uv's
			vertUV.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);
	
			// Add to the positions
			vertPos.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the 9 face indices into an array
			unsigned int i[9];
			sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8]);
	
			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = vertPos[i[0] - 1];
			v1.UV = vertUV[i[1] - 1];
			v1.Normal = vertNrm[i[2] - 1];
	
			Vertex v2;
			v2.Position = vertPos[i[3] - 1];
			v2.UV = vertUV[i[4] - 1];
			v2.Normal = vertNrm[i[5] - 1];
	
			Vertex v3;
			v3.Position = vertPos[i[6] - 1];
			v3.UV = vertUV[i[7] - 1];
			v3.Normal = vertNrm[i[8] - 1];
	
			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;
	
			// Add the verts to the vector
			vertList.push_back(v1);
			vertList.push_back(v2);
			vertList.push_back(v3);
	
			// Add three more indices
			indices.push_back(vertCounter++);
			indices.push_back(vertCounter++);
			indices.push_back(vertCounter++);

			XMVECTOR v0Pos = XMLoadFloat3(&vertPos[i[0]-1]);
			XMVECTOR v1Pos = XMLoadFloat3(&vertPos[i[3]-1]);
			XMVECTOR v2Pos = XMLoadFloat3(&vertPos[i[6]-1]);
			
			XMVECTOR uv0 = XMLoadFloat2(&vertUV[i[1]-1]);
			XMVECTOR uv1 = XMLoadFloat2(&vertUV[i[4]-1]);
			XMVECTOR uv2 = XMLoadFloat2(&vertUV[i[7]-1]);
			
			XMVECTOR deltaPos1 = v1Pos - v0Pos;
			XMVECTOR deltaPos2 = v2Pos - v0Pos;
			
			XMVECTOR deltaU1 = uv1 - uv0;
			XMVECTOR deltaU2 = uv2 - uv0;
			
			XMFLOAT3 dP1,dP2; 
			XMStoreFloat3(&dP1, deltaPos1);
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

	// Close
	obj.close();
	
	vertArray = new Vertex[vertCounter];
	for (int i = 0; i < vertCounter; ++i) {
		vertArray[i] = vertList[i];
	}
	indexArray = new unsigned int[vertCounter];
	for (int i = 0; i < vertCounter; ++i) {
		indexArray[i] = indices[i];
	}
	/*vertArray = &vertList[0];
	indexArray = &indices[0];*/
	
	// - At this point, "verts" is a vector of Vertex structs, and can be used
	//    directly to create a vertex buffer:  &verts[0] is the first vert
	//
	// - The vector "indices" is similar. It's a vector of unsigned ints and
	//    can be used directly for the index buffer: &indices[0] is the first int
	//
	// - "vertCounter" is BOTH the number of vertices and the number of indices
}

void MeshLoader::avgTanBiTan() {
	for (int i = 0; i < indices.size(); i += 3) {
		XMVECTOR xmTan = XMLoadFloat3(&vertTan[indices[i]]);
		xmTan += XMLoadFloat3(&vertTan[indices[i + 1]]);
		xmTan += XMLoadFloat3(&vertTan[indices[i + 2]]);

		xmTan /= 3;

		XMVECTOR xmBiTan = XMLoadFloat3(&vertBiTan[indices[i]]);
		xmBiTan += XMLoadFloat3(&vertBiTan[indices[i + 1]]);
		xmBiTan += XMLoadFloat3(&vertBiTan[indices[i + 2]]);

		xmBiTan /= 3;

		XMFLOAT3 tan, biTan;
		XMStoreFloat3(&tan, xmTan);
		XMStoreFloat3(&biTan, xmBiTan);

		/*vertTanActual.push_back(tan);
		vertBiTanActual.push_back(biTan);*/

		vertTan[indices[i + 0]] = tan;
		vertTan[indices[i + 1]] = tan;
		vertTan[indices[i + 2]] = tan;

		vertBiTan[indices[i + 0]] = biTan;
		vertBiTan[indices[i + 1]] = biTan;
		vertBiTan[indices[i + 2]] = biTan;
	}
	for (int i = 0; i < indices.size(); i++) {
		vertList[indices[i]].Tan = vertTan[indices[i]];
	}
	for (int i = 0; i < indices.size(); i++) {
		vertList[indices[i]].BiTan = vertBiTan[indices[i]];
	}
}

Vertex* MeshLoader::getVerts() {
	return vertArray;
}
unsigned int* MeshLoader::getIndices() {
	return indexArray;
}

unsigned int MeshLoader::getNumVerts() {
	return vertList.size();
}

unsigned int MeshLoader::getNumInd() {
	return indices.size();
}

MeshLoader::~MeshLoader()
{
	delete indexArray;
	delete vertArray;
}
