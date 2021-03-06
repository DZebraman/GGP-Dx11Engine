#include "FXAA.h"

FXAA::FXAA(int width, int height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
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
	edges->setBlur(false);
	edges->setOutlineWidth(1.5f);
}

SRV* FXAA::draw(SRV* ppSRV) {
	SRV* srv = edges->draw(ppSRV);

	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &aaRTV, 0);
	deviceContext->ClearRenderTargetView(aaRTV, color);

	fxaaPS->SetInt("blurAmount", 10);
	fxaaPS->SetShaderResourceView("pixels", ppSRV);
	fxaaPS->SetShaderResourceView("blur", srv);
	fxaaPS->SetFloat("pixelWidth", 1 / windowWidth);
	fxaaPS->SetFloat("pixelHeight", 1 / windowHeight);
	fxaaPS->SetShader();

	deviceContext->Draw(3, 0);

	return aaSRV;
}

FXAA::~FXAA()
{
}
