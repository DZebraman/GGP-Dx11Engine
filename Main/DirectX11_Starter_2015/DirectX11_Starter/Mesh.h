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

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11Device* device;
	ID3D11DeviceContext * deviceContext;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

public:
	Mesh(ID3D11Device* _device, ID3D11DeviceContext * _deviceContext);
	void CreateGeometry(Vertex* vertexArray, unsigned int * indices, int numVerts, int numIndices);
	void CreateGeometry(const char * path);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
	void LoadShaders(ID3D11Device* device, ID3D11DeviceContext * deviceContext);
	void Draw(float aspectRatio);
	void CreateMatrices(float _aspectRatio);
};

