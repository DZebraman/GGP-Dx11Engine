// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "MyDemoGame.h"
#include "Vertex.h"
#include "WICTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;


#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the game object.
	MyDemoGame game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;
	
	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"My Super Fancy GGP Game";

	// Custom window size - will be created by Init() later
	windowWidth = 1280;
	windowHeight = 720;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
	// Release any D3D stuff that's still hanging out
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);

	// Delete our simple shaders
	delete vertexShader;
	delete pixelShader;

	delete camera;
	delete material;

	//delete the meshes
	/*for (int i = 0; i < numMeshes; i++) {
		delete meshes[i];
	}*/
	delete meshes;
	for (int i = 0; i < numMeshes; i++) {
		delete entities[i];
	}
	delete entities;
	//delete mesh1;
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if( !DirectXGameCore::Init() )
		return false;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	LoadShaders(); 
	CreateGeometry();
	CreateMatrices();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	lights = new DirectionalLight[2];

	lights[0] = { XMFLOAT4(0.04f,0,0.08f,1.f),XMFLOAT4(1,1,1,1),XMFLOAT3(0,0.2f,0.7f)};
	lights[1] = { XMFLOAT4(0,0,0,1),XMFLOAT4(0.8,0.2,0.2,1),XMFLOAT3(1,0,0) };

	pixelShader->SetData(
		"light1",
		&lights[0],
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"light2",
		&lights[1],
		sizeof(DirectionalLight));

	camera = new Camera();
	camera->updateProjection(aspectRatio);

	material = new Material(vertexShader, pixelShader);

	CreateWICTextureFromFile(device, deviceContext, L"BrickNormal.jpg", 0, &normalSRV);

	// Create the sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	pixelShader->SetShaderResourceView("normalTexture", normalSRV);
	pixelShader->SetSamplerState("trilinear", samplerState);

	// Successfully initialized
	return true;
}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	meshes = new Mesh*[numMeshes];
	entities = new Entity*[numMeshes];
	for (int i = 0; i < numMeshes; i++) {
		meshes[i] = new Mesh(device, deviceContext);
		entities[i] = new Entity(meshes[i],vertexShader,pixelShader, material);
	}

	XMFLOAT3 pos = XMFLOAT3(-5.f, 0.f, 10.f);
	XMFLOAT3 dir = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT3 up = XMFLOAT3(0.f, 1.f, 0.f);
	XMFLOAT3 scale = XMFLOAT3(1.f, 1.f, 1.f);

	for (int i = 0; i < numMeshes; ++i) {
		entities[i]->setVec("pos", pos);
		entities[i]->setVec("fwd", dir);
		entities[i]->setVec("up", up);
		entities[i]->setVec("scale", scale);
		pos.x += 5;
	}

	
	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	/*Vertex vertices[] = {
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), red },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), blue },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), green },
	};*/
	/*Vertex vertices[] = {
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), red },
		{ XMFLOAT3(+1.5f, +1.0f, +0.0f), blue },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), green },
		{ XMFLOAT3(+0.0f, -1.0f, +0.0f), blue },
	};*/
	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...

	unsigned int indices[] = { 0, 1, 2};

	//meshes[0]->CreateGeometry(vertices, indices, 4,6);
	meshes[0]->CreateGeometry("testModel.obj");

	meshes[1]->CreateGeometry("helix.obj");

	Vertex vertices3[] =
	{
		{ XMFLOAT3(-1.0f, +0.0f, -1.0f), XMFLOAT3(0,0,-1),XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.5f, -2.0f, -1.0f), XMFLOAT3(0,0,-1),XMFLOAT2(0,0) },
		{ XMFLOAT3(-2.5f, -2.0f, -1.0f), XMFLOAT3(0,0,-1),XMFLOAT2(0,0) },
	};

	meshes[2]->CreateGeometry(vertices3, indices, 3,3);
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void MyDemoGame::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!
    
	// Create the View matrix
	// - In an actual game, recreate this matrix when the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction you want it to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,		  	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

	if(camera)
		camera->updateProjection(aspectRatio);
}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
float x = 0;
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	camera->update(deltaTime);

	XMFLOAT3 newPos = entities[1]->getVec("pos");
	newPos.x += sin(totalTime) * deltaTime;

	entities[1]->setVec("pos", newPos);


	XMMATRIX tempRotMat = XMLoadFloat4x4(&entities[0]->getMat("rot"));
	//avoid gimbal lock by using "additive" matricies
	XMFLOAT3 vTempRot(0.f,0.f,0.f);
	if (GetAsyncKeyState('K') & 0x8000) { vTempRot.x += (2 * deltaTime); }
	if (GetAsyncKeyState('I') & 0x8000) { vTempRot.x -= (2 * deltaTime); }
	if (GetAsyncKeyState('L') & 0x8000) { vTempRot.y += (2 * deltaTime); }
	if (GetAsyncKeyState('J') & 0x8000) { vTempRot.y -= (2 * deltaTime); }
	if (GetAsyncKeyState('O') & 0x8000) { vTempRot.z += (2 * deltaTime); }
	if (GetAsyncKeyState('U') & 0x8000) { vTempRot.z -= (2 * deltaTime); }

	//tempRotMat *= XMMatrixRotationX(vTempRot.x) * XMMatrixRotationY(vTempRot.y) * XMMatrixRotationZ(vTempRot.z);
	tempRotMat *= XMMatrixRotationRollPitchYaw(vTempRot.x, vTempRot.y, vTempRot.z);
	if (GetAsyncKeyState('R') & 0x8000) { tempRotMat = XMMatrixIdentity(); }


	XMFLOAT4X4 tempRot;
	XMStoreFloat4x4(&tempRot, XMMatrixTranspose(tempRotMat));

	entities[0]->setMat("rot", tempRot);

	static float e2xRot = 0;
	e2xRot += 1.f * deltaTime;
	tempRotMat = XMLoadFloat4x4(&entities[1]->getMat("rot"));
	tempRotMat = XMMatrixRotationAxis({ 0,1,0 }, e2xRot);

	XMStoreFloat4x4(&tempRot, XMMatrixTranspose(tempRotMat));

	entities[1]->setMat("rot", tempRot);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//code moved to Entity

	for (int i = 0; i < numMeshes; i++) {
		entities[i]->Draw(camera->getViewMatrix(),camera->getProjMatrix());
	}
	

	// Present the buffer
	//  - Puts the image we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME
	//  - Always at the very end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	canMoveCam = true;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	canMoveCam = false;

	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	//only change view with mouse is pressed
	if(canMoveCam)
		camera->mouseRotation((y - prevMousePos.y) * rotScale, (x - prevMousePos.x) * rotScale);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion