#pragma once
#include "Mesh.h"
#include "Material.h"
#include <string>
#include <map>
using namespace DirectX;

class Entity
{

private:
	Mesh* mesh;
	Material* material;

	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	std::map<std::string, XMFLOAT4X4> matLookUp;
	std::map<std::string, XMFLOAT3> vecLookUp;

public:
	Entity(Mesh* _mesh, SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, Material* _material);

	XMFLOAT4X4 getMat(std::string matName);
	void setMat(std::string matName, XMFLOAT4X4 newMat);
	XMFLOAT3 getVec(std::string vecName);
	void setVec(std::string vecName, XMFLOAT3 newVec);
	void PrepareMaterial(XMFLOAT4X4 * viewMat, XMFLOAT4X4 * projMat);
	void Draw(XMFLOAT4X4 * viewMat, XMFLOAT4X4 * projMat);
	~Entity();
};

