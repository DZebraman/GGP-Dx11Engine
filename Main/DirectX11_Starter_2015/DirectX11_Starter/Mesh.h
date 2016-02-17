#pragma once
#include <d3d11.h>
#include "Vertex.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "MeshLoader.h"

class Mesh
{
private:
	ID3D11Buffer *vBuffer, *iBuffer;
	int numIndices;

	ID3D11Device* device;
	ID3D11DeviceContext * deviceContext;

public:
	Mesh(ID3D11Device* _device, ID3D11DeviceContext * _deviceContext);
	void CreateGeometry(Vertex* vertexArray, unsigned int * indices, int numVerts, int numIndices);
	void CreateGeometry(const char * path);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
	void LoadShaders(ID3D11Device* device, ID3D11DeviceContext * deviceContext);
	void Draw();
	void CreateMatrices(float _aspectRatio);
};

