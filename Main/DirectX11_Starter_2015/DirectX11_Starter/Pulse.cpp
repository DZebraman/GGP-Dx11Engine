#include "Pulse.h"
#include "WICTextureLoader.h"


Pulse::Pulse(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
{
	windowWidth = width;
	windowHeight = height;

	device = _device;
	deviceContext = _deviceContext;

	sampler = _sampler;

	pulsePS = new SimplePixelShader(device, deviceContext);
	pulsePS->LoadShaderFile(L"Pulse.cso");

	
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"pulse3.png", 0, &pulseTexture);

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

SRV* Pulse::draw(SRV* ppSRV) {
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &pulseRTV, 0);
	deviceContext->ClearRenderTargetView(pulseRTV, color);

	pulsePS->SetShaderResourceView("pixels", ppSRV);
	pulsePS->SetShaderResourceView("pulse", pulseTexture);
	pulsePS->SetSamplerState("trilinear", sampler);
	pulsePS->SetSamplerState("textureClamp", pulseSampler);
	pulsePS->SetFloat("uvScale", pulseSize);
	pulsePS->SetShader();

	pulseSize -= pulseIterator;
	pulseIterator *= 0.999f;
	if (pulseSize < 0.125f)
	{
		pulseSize = 1; pulseIterator = 0.001f;
	}

	deviceContext->Draw(3, 0);
	pulsePS->SetShaderResourceView("pixels", 0);

	return pulseSRV;
}