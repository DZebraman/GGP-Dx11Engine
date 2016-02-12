#include "Entity.h"



Entity::Entity(Mesh* _mesh, SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader)
{
	mesh = _mesh;

	vertexShader = _vertexShader;
	pixelShader = _pixelShader;

	XMMATRIX xmRot = XMMatrixIdentity();
	XMStoreFloat4x4(&matLookUp["rot"], XMMatrixTranspose(xmRot));
}

void Entity::Draw(float aspectRatio) {

	XMFLOAT3 vPos, vFwd, vUp, vScale;
	XMFLOAT4X4 mWorld, mPos, mRot, mScale;

	vPos = vecLookUp["pos"]; vFwd = vecLookUp["fwd"]; 
	vUp = vecLookUp["up"]; vScale = vecLookUp["scale"];

	mPos = matLookUp["pos"]; mWorld = matLookUp["world"];
	mRot = matLookUp["rot"]; mScale = matLookUp["scale"];

	XMVECTOR xvPos = XMLoadFloat3(&vecLookUp["pos"]);
	XMVECTOR xvFwd = XMLoadFloat3(&vFwd);
	XMVECTOR xvUp = XMLoadFloat3(&vUp);
	XMVECTOR xvScale = XMLoadFloat3(&vecLookUp["scale"]);
	//XMVECTOR xvRight = XMVector3Cross(xvFwd, xvUp);

	XMMATRIX xmWorld = XMMatrixIdentity();

	XMMATRIX xmRot = XMLoadFloat4x4(&matLookUp["rot"]);
	XMStoreFloat4x4(&matLookUp["rot"], XMMatrixTranspose(xmRot));

	XMMATRIX xmPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	XMStoreFloat4x4(&matLookUp["pos"], XMMatrixTranspose(xmPos));

	XMMATRIX xmScale = XMMatrixScalingFromVector(xvScale);
	XMStoreFloat4x4(&matLookUp["scale"], XMMatrixTranspose(xmScale));

	//xmWorld = XMMatrixMultiply(xmPos, XMMatrixMultiply(xmRot, xmScale));
	xmWorld = xmRot * xmPos;
	XMStoreFloat4x4(&matLookUp["world"], XMMatrixTranspose(xmWorld));

	//code moved to the Mesh class for organization
	vertexShader->SetMatrix4x4("world", mWorld);

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader(true);
	pixelShader->SetShader(true);

	mesh->Draw(aspectRatio);
}

Entity::~Entity()
{
	delete mesh;
	mesh = nullptr;
}

//uses map to consolidate getters and setters
XMFLOAT4X4 Entity::getMat(std::string matName) {return matLookUp[matName];}
void Entity::setMat(std::string matName, XMFLOAT4X4 newMat) {matLookUp[matName] = newMat;}
XMFLOAT3 Entity::getVec(std::string vecName) {return vecLookUp[vecName];}
void Entity::setVec(std::string vecName, XMFLOAT3 newVec) {vecLookUp[vecName] = newVec;}