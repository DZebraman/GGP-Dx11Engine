#include "PostProcess.h"



PostProcess::PostProcess(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11SamplerState* _sampler, ID3D11DepthStencilView* _depthStencilView)
{
	sampler = _sampler;
	device = _device;
	deviceContext = _deviceContext;
	depthStencilView = _depthStencilView;
	ppVS = new SimpleVertexShader(device, deviceContext);
	ppVS->LoadShaderFile(L"BlurVS.cso");
	ppPS = new SimplePixelShader(device, deviceContext);
	ppPS->LoadShaderFile(L"Final.cso");
}


PostProcess::~PostProcess()
{
}

void PostProcess::AddEffect(PostProcessBase* effect) { ppChain.push_back(effect); }

void PostProcess::draw(ID3D11ShaderResourceView* ppSRV, ID3D11RenderTargetView* renderTargetView) {

	ID3D11ShaderResourceView* srv;

	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };
	//D3D11_SAMPLER_DESC samplerDesc = {};

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// Turn off existing vert/index buffers
	ID3D11Buffer* nothing = 0;
	deviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	deviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	ppVS->SetShader();

	//for (int i = 0; i < ppChain.size(); ++i) {
		ppChain[0]->draw(ppSRV,depthStencilView);
	//}

	// Reset states
	deviceContext->RSSetState(0);
	deviceContext->OMSetDepthStencilState(0, 0);

	// Done with "regular" rendering - swap to post process
	deviceContext->OMSetRenderTargets(1, &renderTargetView, 0);
	deviceContext->ClearRenderTargetView(renderTargetView, color);


	ppPS->SetShaderResourceView("pixels", ppSRV);
	//ppPS->SetShader();

	// Finally - DRAW!
	deviceContext->Draw(3, 0);
}