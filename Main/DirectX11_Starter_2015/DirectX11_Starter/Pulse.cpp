#include "Pulse.h"
#include "WICTextureLoader.h"


Pulse::Pulse(int width, int height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
{
	windowWidth = width;
	windowHeight = height;
	aspectRatio = width / height;

	device = _device;
	deviceContext = _deviceContext;

	sampler = _sampler;

	pulsePS = new SimplePixelShader(device, deviceContext);
	pulsePS->LoadShaderFile(L"Pulse.cso");

	
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"sidePulse.png", 0, &pulseTexture);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"grid.png", 0, &gridTexture);

	setupRenderTarget(&pulseRTV, &pulseSRV);

	// Create the sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &pulseSampler);
}


Pulse::~Pulse()
{
	ReleaseMacro(pulseRTV);
	ReleaseMacro(pulseSRV);
	ReleaseMacro(pulseTexture);
	delete pulsePS;
}

void Pulse::setDeltaTime(float dt) {
	deltaTime = dt;
}

SRV* Pulse::draw(SRV* ppSRV) {
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &pulseRTV, 0);
	deviceContext->ClearRenderTargetView(pulseRTV, color);

	pulsePS->SetShaderResourceView("grid", gridTexture);
	pulsePS->SetShaderResourceView("pixels", ppSRV);
	pulsePS->SetShaderResourceView("pulse", pulseTexture);
	pulsePS->SetSamplerState("trilinear", sampler);
	pulsePS->SetSamplerState("textureClamp", pulseSampler);
	pulsePS->SetFloat("uvScale", 1);
	pulsePS->SetFloat("offSetX",pulseSize);
	pulsePS->SetFloat("aspectRatio", aspectRatio);
	pulsePS->SetShader();

	pulseSize += pulseIterator * deltaTime;
	//pulseSize = fmodf(pulseSize, 0.5f) - 0.5f;

	deviceContext->Draw(3, 0);
	pulsePS->SetShaderResourceView("pixels", 0);

	return pulseSRV;
}