#include "EdgeDetect.h"



EdgeDetect::EdgeDetect(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext)
{
	device = _device;
	deviceContext = _deviceContext;

	windowWidth = width;
	windowHeight = height;

	edgePS = new SimplePixelShader(device, deviceContext);
	edgePS->LoadShaderFile(L"EdgePS.cso");

	blurPS = new SimplePixelShader(device, deviceContext);
	blurPS->LoadShaderFile(L"BlurPS.cso");

	setupRenderTarget(&edgeRTV, &edgeSRV);
	setupRenderTarget(&blurRTV, &blurSRV);
}


EdgeDetect::~EdgeDetect()
{
}

SRV* EdgeDetect::draw(SRV* ppSRV) {
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &edgeRTV, 0);
	deviceContext->ClearRenderTargetView(edgeRTV, color);

	blurPS->SetInt("blurAmount", 10);
	blurPS->SetFloat("pixelWidth", 1.0f / windowWidth);
	blurPS->SetFloat("pixelHeight", 1.0f / windowHeight);
	blurPS->SetShaderResourceView("pixels", ppSRV);
	blurPS->SetSamplerState("trilinear", sampler);
	blurPS->SetShader();

	deviceContext->Draw(3, 0);

	deviceContext->OMSetRenderTargets(1, &edgeRTV, 0);
	deviceContext->ClearRenderTargetView(edgeRTV, color);

	edgePS->SetShaderResourceView("pixels", ppSRV);
	edgePS->SetFloat("thresholdMin", 0.1f);
	edgePS->SetFloat("thresholdMax", 0.5f);
	edgePS->SetFloat("pixelWidth", windowWidth);
	edgePS->SetFloat("pixelHeight", windowHeight);
	edgePS->SetShader();

	deviceContext->Draw(3, 0);

	return edgeSRV;
}