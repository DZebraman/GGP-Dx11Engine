#pragma once
#include <vector>
#include "PostProcessBase.h"
#include "Blur.h"
#include "Bloom.h"
#include "Tint.h"
#include "Vertex.h"
#include "ppTest.h"
#include "ScreenDepth.h"
#include "EdgeDetect.h"
#include "FXAA.h"
#include "ChromaticAberation.h"
#include "Pulse.h"
class PostProcess
{
public:
	PostProcess(ID3D11Device* _device,ID3D11DeviceContext* _deviceContext, ID3D11SamplerState* _sampler, ID3D11DepthStencilView* _depthStencilView);
	~PostProcess();
	void draw(ID3D11ShaderResourceView* ppSRV, ID3D11RenderTargetView* renderTargetView, float deltaTime);
	void AddEffect(PostProcessBase* effect);
private:
	std::vector<PostProcessBase*> ppChain;
	SimpleVertexShader* ppVS;
	SimplePixelShader* ppPS;
	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;
	ID3D11SamplerState* sampler;

	ID3D11DepthStencilView* depthStencilView;
};

