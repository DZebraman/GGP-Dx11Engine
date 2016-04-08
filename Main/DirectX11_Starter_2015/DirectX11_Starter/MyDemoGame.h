#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include "PostProcess.h"

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define numMeshes 3
#define rotScale 0.0005f

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime, float totalTime);
	void DrawScene(float deltaTime, float totalTime);

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	bool canMoveCam = false;

private:
	// Initialization for our "game" demo - Feel free to
	// expand, alter, rename or remove these once you
	// start doing something more advanced!
	void LoadShaders(); 
	void CreateGeometry();
	void CreateMatrices();

	Entity** entities;
	Mesh** meshes;
	Camera* camera;
	Material* material;
	Material* material2;
	Material* simpleMaterial;

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimplePixelShader* pixelShader2;
	SimplePixelShader* simplePS;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	//PostProcess
	ID3D11RenderTargetView* ppRTV;
	ID3D11ShaderResourceView* ppSRV;
	PostProcess* ppChain;

	Blur* blurEffect;
	Blur* blurEffect2;
	Bloom* bloomEffect;
	Tint* tintEffect;
	ppTest* testEffect;
	ScreenDepth* screenDepth;
	EdgeDetect* edgeDetect;

	//texture 
	ID3D11ShaderResourceView* skySRV;
	ID3D11ShaderResourceView* normalSRV;
	ID3D11ShaderResourceView* glossSRV;
	ID3D11ShaderResourceView* diffSRV;
	ID3D11ShaderResourceView* specSRV;
	ID3D11SamplerState* samplerState;

	DirectionalLight* lights;
	//DirectionalLight sceneLight;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};