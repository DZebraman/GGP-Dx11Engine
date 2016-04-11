#include "FXAA.h"

FXAA::FXAA(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
{
	windowWidth = width;
	windowHeight = height;

	device = _device;
	deviceContext = _deviceContext;

	sampler = _sampler;

	fxaaPS = new SimplePixelShader(device, deviceContext);
	fxaaPS->LoadShaderFile(L"FXAA.cso");

	//Sets up the render targets and srv's for both passes
	setupRenderTarget(&aaRTV, &aaSRV);

	edges = new EdgeDetect(width, height, device, deviceContext);
}

SRV* FXAA::draw(SRV* ppSRV) {
	SRV* srv = edges->draw(ppSRV);

	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &aaRTV, 0);
	deviceContext->ClearRenderTargetView(aaRTV, color);

	fxaaPS->SetShaderResourceView("pixels", ppSRV);
	fxaaPS->SetFloat("thresholdMin", 0.1f);
	fxaaPS->SetFloat("thresholdMax", 0.5f);
	fxaaPS->SetFloat("pixelWidth", windowWidth);
	fxaaPS->SetFloat("pixelHeight", windowHeight);
	fxaaPS->SetShader();

	deviceContext->Draw(3, 0);

	return aaSRV;
}

FXAA::~FXAA()
{
}
