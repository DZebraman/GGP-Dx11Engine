#include "ScreenDepth.h"



ScreenDepth::ScreenDepth(int width, int height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler, Camera* _camera, Entity** _entities, int _numMeshes) {
	device = _device;
	deviceContext = _deviceContext;

	windowWidth = width;
	windowHeight = height;

	entities = _entities;
	numMeshes = _numMeshes;

	sampler = _sampler;

	depthVS = new SimpleVertexShader(device, deviceContext);
	depthVS->LoadShaderFile(L"SceneDepthVS.cso");

	depthPS = new SimplePixelShader(device, deviceContext);
	depthPS->LoadShaderFile(L"SceneDepthPS.cso");

	camera = _camera;

	setupRenderTarget(&depthRTV, &depthSRV);
}

SRV* ScreenDepth::draw(SRV* ppSRV) {
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &depthRTV, 0);
	deviceContext->ClearRenderTargetView(depthRTV, color);

	depthPS->SetShader();

	for (int i = 0; i < numMeshes; ++i) {
		entities[i]->DrawNoMaterial(camera->getViewMatrix(), camera->getProjMatrix());
	}

	return depthSRV;
}

ScreenDepth::~ScreenDepth()
{
}


