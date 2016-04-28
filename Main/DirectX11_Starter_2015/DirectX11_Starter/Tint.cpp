#include "Tint.h"



Tint::Tint(int width, int height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler, ID3D11RenderTargetView* _rtv)
{
	device = _device;
	deviceContext = _deviceContext;

	windowWidth = width;
	windowHeight = height;

	sampler = _sampler;

	ppVS = new SimpleVertexShader(device, deviceContext);
	ppVS->LoadShaderFile(L"BlurVS.cso");

	ppPS = new SimplePixelShader(device, deviceContext);
	ppPS->LoadShaderFile(L"Tint.cso");

	//blurRTV = _rtv;

	setupRenderTarget(&tintRTV, &tintSRV);

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
	//device->CreateRenderTargetView(ppTexture, &rtvDesc, &tintRTV);

	////Make an SRV 
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = tDesc.Format;
	//srvDesc.Texture2D.MipLevels = 1;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//device->CreateShaderResourceView(ppTexture, &srvDesc, &tintSRV);

	////Get rid of ONE of the texture references
	//ppTexture->Release();
}

ID3D11ShaderResourceView* Tint::draw(ID3D11ShaderResourceView* ppSRV){
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	// Reset states

	deviceContext->OMSetRenderTargets(1, &tintRTV, 0);
	deviceContext->ClearRenderTargetView(tintRTV, color);

	ppPS->SetShaderResourceView("pixels", ppSRV);
	ppPS->SetShader();

	deviceContext->Draw(3, 0);
	ppPS->SetShaderResourceView("pixels", 0);

	return tintSRV;
}

Tint::~Tint()
{
}
