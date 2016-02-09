#include "Entity.h"



Entity::Entity(Mesh* _mesh)
{
	mesh = _mesh;

	matLookUp["world"] = &mWorld;
	matLookUp["pos"] = &mPos;
	matLookUp["rot"] = &mRot;
	matLookUp["scale"] = &mScale;

	vecLookUp["pos"] = &vPos;
	vecLookUp["fwd"] = &vFwd;
	vecLookUp["up"] = &vUp;

}

void Entity::Draw(float aspectRatio) {
	mesh->Draw(aspectRatio);
}

Entity::~Entity()
{
	delete mesh;
	mesh = nullptr;
}

XMFLOAT4X4 Entity::getMat(std::string matName) {
	return *matLookUp[matName];
}

void Entity::setMat(std::string matName, XMFLOAT4X4 newMat) {
	matLookUp[matName] = &newMat;
}

XMFLOAT3 Entity::getVec(std::string vecName) {
	return *vecLookUp[vecName];
}

void Entity::setVec(std::string vecName, XMFLOAT3 newVec) {
	vecLookUp[vecName] = &newVec;
}