#include "Bloom.h"



Bloom::Bloom(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
{
	windowWidth = width;
	windowHeight = height;

	device = _device;
	deviceContext = _deviceContext;

	sampler = _sampler;

	filterVS = new SimpleVertexShader(device, deviceContext);

	compositePS = new SimplePixelShader(device, deviceContext);
	compositePS->LoadShaderFile(L"BloomComposite.cso");

	filterPS = new SimplePixelShader(device, deviceContext);
	filterPS->LoadShaderFile(L"BloomFilter.cso");

	//// Create a texture
	//D3D11_TEXTURE2D_DESC tDesc = {};
	//tDesc.Width = width;
	//tDesc.Height = height;
	//tDesc.ArraySize = 1;
	//tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//tDesc.CPUAccessFlags = 0;
	//tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//tDesc.MipLevels = 1;
	//tDesc.MiscFlags = 0;
	//tDesc.SampleDesc.Count = 1;
	//tDesc.SampleDesc.Quality = 0;
	//tDesc.Usage = D3D11_USAGE_DEFAULT;
	//ID3D11Texture2D* ppTexture;
	//device->CreateTexture2D(&tDesc, 0, &ppTexture);

	//// Make a render target view for rendering into the texture
	//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	//rtvDesc.Format = tDesc.Format;
	//rtvDesc.Texture2D.MipSlice = 0;
	//rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//device->CreateRenderTargetView(ppTexture, &rtvDesc, &filterRTV);

	////Make an SRV 
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = tDesc.Format;
	//srvDesc.Texture2D.MipLevels = 1;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//device->CreateShaderResourceView(ppTexture, &srvDesc, &filterSRV);

	////Get rid of ONE of the texture references
	//ppTexture->Release();

	//// Create a texture
	//D3D11_TEXTURE2D_DESC tDesc2 = {};
	//tDesc2.Width = width;
	//tDesc2.Height = height;
	//tDesc2.ArraySize = 1;
	//tDesc2.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//tDesc2.CPUAccessFlags = 0;
	//tDesc2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//tDesc2.MipLevels = 1;
	//tDesc2.MiscFlags = 0;
	//tDesc2.SampleDesc.Count = 1;
	//tDesc2.SampleDesc.Quality = 0;
	//tDesc2.Usage = D3D11_USAGE_DEFAULT;
	//ID3D11Texture2D* ppTexture2;
	//device->CreateTexture2D(&tDesc2, 0, &ppTexture2);

	//// Make a render target view for rendering into the texture
	//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc2 = {};
	//rtvDesc2.Format = tDesc2.Format;
	//rtvDesc2.Texture2D.MipSlice = 0;
	//rtvDesc2.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//device->CreateRenderTargetView(ppTexture2, &rtvDesc2, &compositeRTV);

	////Make an SRV 
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc2 = {};
	//srvDesc2.Format = tDesc2.Format;
	//srvDesc2.Texture2D.MipLevels = 1;
	//srvDesc2.Texture2D.MostDetailedMip = 0;
	//srvDesc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//device->CreateShaderResourceView(ppTexture2, &srvDesc2, &compositeSRV);

	////Get rid of ONE of the texture references
	//ppTexture2->Release();

	//Sets up the render targets and srv's for both passes
	setupRenderTarget(&filterRTV, &filterSRV);
	setupRenderTarget(&compositeRTV, &compositeSRV);
}


Bloom::~Bloom()
{
}

ID3D11ShaderResourceView* Bloom::draw(ID3D11ShaderResourceView* ppSRV){
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &filterRTV, 0);
	deviceContext->ClearRenderTargetView(filterRTV, color);

	filterPS->SetFloat("minBrightness", 0.6f);
	filterPS->SetShaderResourceView("pixels", ppSRV);
	filterPS->SetSamplerState("trilinear", sampler);
	filterPS->SetShader();

	deviceContext->Draw(3, 0);
	filterPS->SetShaderResourceView("pixels", 0);

	deviceContext->OMSetRenderTargets(1, &compositeRTV, 0);
	deviceContext->ClearRenderTargetView(compositeRTV, color);

	//return testSRV;

	compositePS->SetInt("blurAmount", 2);
	compositePS->SetFloat("pixelWidth", 1.0f / windowWidth);
	compositePS->SetFloat("pixelHeight", 1.0f / windowHeight);
	compositePS->SetShaderResourceView("test", filterSRV);
	compositePS->SetShaderResourceView("pixels", ppSRV);
	compositePS->SetSamplerState("trilinear", sampler);
	compositePS->SetShader();

	deviceContext->Draw(3, 0);
	compositePS->SetShaderResourceView("pixels", 0);

	return compositeSRV;
}