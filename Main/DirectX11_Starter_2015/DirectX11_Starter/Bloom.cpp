#include "Bloom.h"



Bloom::Bloom(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
{
	windowWidth = width;
	windowHeight = height;

	device = _device;
	deviceContext = _deviceContext;

	sampler = _sampler;

	compositePS = new SimplePixelShader(device, deviceContext);
	compositePS->SetShader(L"BloomComposite.cso");

	filterPS = new SimplePixelShader(device, deviceContext);
	filterPS->SetShader(L"BloomFilter.cso");

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
	device->CreateRenderTargetView(ppTexture, &rtvDesc, &bloomFilterTarget);

	// Make an SRV 
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = tDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(ppTexture, &srvDesc, &bloomFilterSRV);

	ppTexture->Release();

	// Create a texture
	D3D11_TEXTURE2D_DESC t2Desc = {};
	t2Desc.Width = windowWidth;
	t2Desc.Height = windowHeight;
	t2Desc.ArraySize = 1;
	t2Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	t2Desc.CPUAccessFlags = 0;
	t2Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	t2Desc.MipLevels = 1;
	t2Desc.MiscFlags = 0;
	t2Desc.SampleDesc.Count = 1;
	t2Desc.SampleDesc.Quality = 0;
	t2Desc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* ppTexture2;
	device->CreateTexture2D(&t2Desc, 0, &ppTexture2);

	// Make a render target view for rendering into the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc2 = {};
	rtvDesc2.Format = tDesc.Format;
	rtvDesc2.Texture2D.MipSlice = 0;
	rtvDesc2.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(ppTexture2, &rtvDesc2, &compositeTarget);

	// Make an SRV 
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc2 = {};
	srvDesc2.Format = t2Desc.Format;
	srvDesc2.Texture2D.MipLevels = 1;
	srvDesc2.Texture2D.MostDetailedMip = 0;
	srvDesc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(ppTexture2, &srvDesc2, &compositeSRV);

	ppTexture2->Release();
}


Bloom::~Bloom()
{
}

ID3D11ShaderResourceView* Bloom::draw(ID3D11ShaderResourceView* ppSRV, ID3D11DepthStencilView* depthStencilView) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &bloomFilterTarget, depthStencilView);

	deviceContext->ClearRenderTargetView(bloomFilterTarget, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//ppVS->SetShader();

	filterPS->SetInt("blurAmount", 5);
	filterPS->SetFloat("pixelWidth", 1.0f / windowWidth);
	filterPS->SetFloat("pixelHeight", 1.0f / windowHeight);
	filterPS->SetShaderResourceView("pixels", ppSRV);
	filterPS->SetSamplerState("trilinear", sampler);
	filterPS->SetShader();

	deviceContext->Draw(3, 0);

	return bloomFilterSRV;
}