#include "Entity.h"



Entity::Entity(Mesh* _mesh, SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, Material * _material)
{
	mesh = _mesh;

	vertexShader = _vertexShader;
	pixelShader = _pixelShader;

	material = _material;

	XMMATRIX xmRot = XMMatrixIdentity();
	XMStoreFloat4x4(&matLookUp["rot"], XMMatrixTranspose(xmRot));
}

void Entity::Draw(XMFLOAT4X4 * viewMat, XMFLOAT4X4 * projMat) {

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


	PrepareMaterial(viewMat, projMat);


	mesh->Draw();
}

void Entity::PrepareMaterial(XMFLOAT4X4 * viewMat, XMFLOAT4X4 * projMat) {

	vertexShader->SetMatrix4x4("view", *viewMat);
	vertexShader->SetMatrix4x4("projection", *projMat);
	vertexShader->SetMatrix4x4("world", matLookUp["world"]);

	vertexShader->SetShader(true);
	pixelShader->SetShader(true);
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