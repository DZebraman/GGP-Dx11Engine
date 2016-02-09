#include "Mesh.h"
#include <string>
#include <map>
#pragma once

using namespace DirectX;

class Entity
{

private:
	Mesh* mesh;
	XMFLOAT3 vPos, vFwd, vUp;
	XMFLOAT4X4 mWorld, mPos, mRot, mScale;
	std::map<std::string, XMFLOAT4X4 *> matLookUp;
	std::map<std::string, XMFLOAT3 *> vecLookUp;

public:
	Entity(Mesh* _mesh);

	XMFLOAT4X4 getMat(std::string matName);
	void setMat(std::string matName, XMFLOAT4X4 newMat);
	XMFLOAT3 getVec(std::string vecName);
	void setVec(std::string vecName, XMFLOAT3 newVec);

	void Draw(float aspectRatio);
	~Entity();
};

