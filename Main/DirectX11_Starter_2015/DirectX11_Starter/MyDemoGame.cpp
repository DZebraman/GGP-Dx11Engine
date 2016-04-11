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
#include "DDSTextureLoader.h"

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
	

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	lights = new DirectionalLight[2];

	lights[0] = { XMFLOAT4(0.02f,0,0.04f,1.f),XMFLOAT4(1,1,1,1),XMFLOAT3(0,0,1)};
	lights[1] = { XMFLOAT4(0,0,0,1),XMFLOAT4(0.8,0.2,0.2,1),XMFLOAT3(1,0,0) };

	pixelShader->SetData(
		"light1",
		&lights[0],
		sizeof(DirectionalLight));

	simplePS->SetData(
		"light1",
		&lights[0],
		sizeof(DirectionalLight));

	pixelShader->SetData(
		"light2",
		&lights[1],
		sizeof(DirectionalLight));

	camera = new Camera();
	camera->updateProjection(aspectRatio);

	/*CreateWICTextureFromFile(device, deviceContext, L"Test/metal_normal.png", 0, &normalSRV);
	CreateWICTextureFromFile(device, deviceContext, L"Test/metal_specular.png", 0, &specSRV);
	CreateWICTextureFromFile(device, deviceContext, L"Test/metal_diffuse.png", 0, &diffSRV);
	CreateWICTextureFromFile(device, deviceContext, L"Test/metal_gloss.png", 0, &glossSRV);*/
	CreateWICTextureFromFile(device, deviceContext, L"wood_nrm.png", 0, &normalSRV);
	CreateWICTextureFromFile(device, deviceContext, L"wood_spec.png", 0, &specSRV);
	CreateWICTextureFromFile(device, deviceContext, L"wood_diff.png", 0, &diffSRV);
	CreateWICTextureFromFile(device, deviceContext, L"wood_gloss.png", 0, &glossSRV);

	//DirectX::CreateDDSTextureFromFile(device, deviceContext, L"TestCubeMap.dds", 0, &skySRV);
	DirectX::CreateDDSTextureFromFile(device, deviceContext, L"SunnyCubeMap.dds", 0, &skySRV);

	// Create the sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	material = new Material(vertexShader, pixelShader, samplerState);
	material2 = new Material(vertexShader, pixelShader2, samplerState);
	simpleMaterial = new Material(vertexShader,simplePS,samplerState);

	//sends the srvs to the material
	material->setSRV("nrm2", normalSRV);
	material->setSRV("specTexture", specSRV);
	material->setSRV("diffTexture", diffSRV);
	material->setSRV("glossTexture", glossSRV);
	material->setSRV("cubeMap", skySRV);
	material->setup();

	material2->setSRV("specTexture", specSRV);
	material2->setup();

	simpleMaterial->setSRV("diffTexture", diffSRV);
	simpleMaterial->setup();

	CreateGeometry();
	CreateMatrices();

	ppChain = new PostProcess(device, deviceContext,samplerState,depthStencilView);

	blurEffect = new Blur(windowWidth, windowHeight, device, deviceContext, samplerState,ppRTV);
	blurEffect2 = new Blur(windowWidth, windowHeight, device, deviceContext, samplerState,ppRTV);
	bloomEffect = new Bloom(windowWidth, windowHeight, device, deviceContext, samplerState);
	tintEffect = new Tint(windowWidth, windowHeight, device, deviceContext, samplerState, ppRTV);
	testEffect = new ppTest(windowWidth, windowHeight, device, deviceContext, samplerState, ppRTV);
	screenDepth = new ScreenDepth(windowWidth, windowHeight, device, deviceContext, samplerState, camera, entities, numMeshes);
	edgeDetect = new EdgeDetect(windowWidth, windowHeight, device, deviceContext);
	fxaa = new FXAA(windowWidth, windowHeight, device, deviceContext, samplerState);
	ppChain->AddEffect(fxaa);
	//ppChain->AddEffect(screenDepth);
	//ppChain->AddEffect(edgeDetect);
	//ppChain->AddEffect(bloomEffect);
	//ppChain->AddEffect(blurEffect);
	//ppChain->AddEffect(tintEffect);
	//ppChain->AddEffect(testEffect);
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

	pixelShader2 = new SimplePixelShader(device, deviceContext);
	pixelShader2->LoadShaderFile(L"PixelShader2.cso");

	simplePS = new SimplePixelShader(device, deviceContext);
	simplePS->LoadShaderFile(L"simplePS.cso");

	// Create a texture
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Width = windowWidth;
	tDesc.Height = windowHeight;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = 0;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.MipLevels = 1;
	tDesc.MiscFlags = 0;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* ppTexture;
	device->CreateTexture2D(&tDesc, 0, &ppTexture);

	// Make a render target view for rendering into the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = tDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(ppTexture, &rtvDesc, &ppRTV);

	// Make an SRV 
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = tDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(ppTexture, &srvDesc, &ppSRV);

	// Get rid of ONE of the texture references
	ppTexture->Release();
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
	//for (int i = 0; i < numMeshes; i++) {
	//	meshes[i] = new Mesh(device, deviceContext);
	//	if(i == 1)
	//		entities[i] = new Entity(meshes[i], vertexShader, pixelShader2, material2);
	//	else
	//		entities[i] = new Entity(meshes[i],vertexShader,pixelShader, material);
	//}

	meshes[0] = new Mesh(device, deviceContext);
	entities[0] = new Entity(meshes[0], vertexShader, pixelShader, material);

	meshes[1] = new Mesh(device, deviceContext);
	entities[1] = new Entity(meshes[1], vertexShader, simplePS, simpleMaterial);

	meshes[2] = new Mesh(device, deviceContext);
	entities[2] = new Entity(meshes[2], vertexShader, simplePS, simpleMaterial);

	/*entities[1]->setMaterial(material2);
	entities[2]->setMaterial(simpleMaterial);*/

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

	unsigned int indices[] = { 0, 1, 2};

	//meshes[0]->CreateGeometry(vertices, indices, 4,6);
	meshes[0]->CreateGeometry("helix.obj");

	meshes[1]->CreateGeometry("sphere.obj");

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

	//Use this here for movement. feed in an XMFLOAT3 and all should be well.

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
	tempRotMat = XMMatrixRotationRollPitchYaw(vTempRot.x, vTempRot.y, vTempRot.z) * tempRotMat;
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
	pixelShader->SetFloat3("CameraPosition", camera->getPos());
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.1f, 0.1f, 0.1f, 0.1f};

	// Swap to the new render target
	//deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	deviceContext->OMSetRenderTargets(1, &ppRTV, depthStencilView);

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	//deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearRenderTargetView(ppRTV, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//code moved to Entity

	for (int i = 0; i < numMeshes; i++) {
		entities[i]->Draw(camera->getViewMatrix(),camera->getProjMatrix());
	}

	//// Reset states
	//deviceContext->RSSetState(0);
	//deviceContext->OMSetDepthStencilState(0, 0);

	//// Done with "regular" rendering - swap to post process
	//deviceContext->OMSetRenderTargets(1, &renderTargetView, 0);
	//deviceContext->ClearRenderTargetView(renderTargetView, color);

	ppChain->draw(ppSRV,renderTargetView);

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
		camera->setRotation((y - prevMousePos.y) * rotScale, (x - prevMousePos.x) * rotScale);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion