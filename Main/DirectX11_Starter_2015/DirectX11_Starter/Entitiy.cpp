#include "Entity.h"



Entity::Entity(Mesh* _mesh, SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader)
{
	mesh = _mesh;

	vertexShader = _vertexShader;

	matLookUp["world"] = &mWorld;
	matLookUp["pos"] = &mPos;
	matLookUp["rot"] = &mRot;
	matLookUp["scale"] = &mScale;

	vecLookUp["pos"] = &vPos;
	vecLookUp["fwd"] = &vFwd;
	vecLookUp["up"] = &vUp;

}

void Entity::Draw(float aspectRatio) {

	XMVECTOR xvPos = XMLoadFloat3(&vPos);
	XMVECTOR xvFwd = XMLoadFloat3(&vFwd);
	XMVECTOR xvUp = XMLoadFloat3(&vUp);
	//XMVECTOR xvRight = XMVector3Cross(xvFwd, xvUp);

	XMMATRIX xmWorld = XMMatrixIdentity();

	XMMATRIX xmRot = XMMatrixIdentity();//XMMatrixLookToLH(xvPos, xvFwd, xvUp);
	XMStoreFloat4x4(&mRot, XMMatrixTranspose(xmRot));

	XMMATRIX xmPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	XMStoreFloat4x4(&mPos, XMMatrixTranspose(xmPos));

	XMMATRIX xmScale = XMLoadFloat4x4(&mScale);
	XMStoreFloat4x4(&mScale, XMMatrixTranspose(xmScale));

	xmWorld = XMMatrixMultiply(xmPos, XMMatrixMultiply(xmRot, xmScale));
	XMStoreFloat4x4(&mWorld, XMMatrixTranspose(xmWorld));

	//code moved to the Mesh class for organization
	vertexShader->SetMatrix4x4("world", mWorld);

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader(true);

	mesh->Draw(aspectRatio);
}

Entity::~Entity()
{
	delete mesh;
	mesh = nullptr;
}

//uses map to consolidate getters and setters
XMFLOAT4X4 Entity::getMat(std::string matName) {return *matLookUp[matName];}
void Entity::setMat(std::string matName, XMFLOAT4X4 newMat) {matLookUp[matName] = &newMat;}
XMFLOAT3 Entity::getVec(std::string vecName) {return *vecLookUp[vecName];}
void Entity::setVec(std::string vecName, XMFLOAT3 newVec) {vecLookUp[vecName] = &newVec;}