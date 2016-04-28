#include "ppTest.h"



ppTest::ppTest(int width, int height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler, ID3D11RenderTargetView* _rtv)
{
	device = _device;
	deviceContext = _deviceContext;

	windowWidth = width;
	windowHeight = height;

	sampler = _sampler;

	testVS = new SimpleVertexShader(device, deviceContext);
	testVS->LoadShaderFile(L"BlurVS.cso");

	testPS = new SimplePixelShader(device, deviceContext);
	testPS->LoadShaderFile(L"BloomFilter.cso");

	testPS2 = new SimplePixelShader(device, deviceContext);
	testPS2->LoadShaderFile(L"BloomComposite.cso");

	//blurRTV = _rtv;

	// Create a texture
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Width = width;
	tDesc.Height = height;
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
	device->CreateRenderTargetView(ppTexture, &rtvDesc, &testRTV);

	//Make an SRV 
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = tDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(ppTexture, &srvDesc, &testSRV);

	//Get rid of ONE of the texture references
	ppTexture->Release();

	// Create a texture
	D3D11_TEXTURE2D_DESC tDesc2 = {};
	tDesc2.Width = width;
	tDesc2.Height = height;
	tDesc2.ArraySize = 1;
	tDesc2.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc2.CPUAccessFlags = 0;
	tDesc2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc2.MipLevels = 1;
	tDesc2.MiscFlags = 0;
	tDesc2.SampleDesc.Count = 1;
	tDesc2.SampleDesc.Quality = 0;
	tDesc2.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* ppTexture2;
	device->CreateTexture2D(&tDesc2, 0, &ppTexture2);

	// Make a render target view for rendering into the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc2 = {};
	rtvDesc2.Format = tDesc2.Format;
	rtvDesc2.Texture2D.MipSlice = 0;
	rtvDesc2.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(ppTexture2, &rtvDesc2, &testRTV2);

	//Make an SRV 
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc2 = {};
	srvDesc2.Format = tDesc2.Format;
	srvDesc2.Texture2D.MipLevels = 1;
	srvDesc2.Texture2D.MostDetailedMip = 0;
	srvDesc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(ppTexture2, &srvDesc2, &testSRV2);

	//Get rid of ONE of the texture references
	ppTexture2->Release();
}

ID3D11ShaderResourceView* ppTest::draw(ID3D11ShaderResourceView* ppSRV) {
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &testRTV, 0);
	deviceContext->ClearRenderTargetView(testRTV, color);

	testPS->SetFloat("minBrightness", 0.6f);
	testPS->SetShaderResourceView("pixels", ppSRV);
	testPS->SetSamplerState("trilinear", sampler);
	testPS->SetShader();

	deviceContext->Draw(3, 0);
	testPS->SetShaderResourceView("pixels", 0);

	deviceContext->OMSetRenderTargets(1, &testRTV2, 0);
	deviceContext->ClearRenderTargetView(testRTV2, color);

	//return testSRV;

	testPS2->SetInt("blurAmount", 5);
	testPS2->SetFloat("pixelWidth", 1.0f / windowWidth);
	testPS2->SetFloat("pixelHeight", 1.0f / windowHeight);
	testPS2->SetShaderResourceView("test", testSRV);
	testPS2->SetShaderResourceView("pixels", ppSRV);
	testPS2->SetSamplerState("trilinear", sampler);
	testPS2->SetShader();

	deviceContext->Draw(3, 0);
	testPS2->SetShaderResourceView("pixels", 0);

	return testSRV2;
}

ppTest::~ppTest()
{
}
