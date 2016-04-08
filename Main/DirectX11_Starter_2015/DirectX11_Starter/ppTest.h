#pragma once
#include "PostProcessBase.h"
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Vertex.h"
class ppTest : public PostProcessBase
{
public:
	ppTest(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler, ID3D11RenderTargetView* _rtv);
	virtual ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV);
	~ppTest();
private:
	// Post process stuff
	ID3D11RenderTargetView* testRTV;
	ID3D11ShaderResourceView* testSRV;
	SimpleVertexShader* testVS;
	SimplePixelShader* testPS;

	ID3D11RenderTargetView* testRTV2;
	ID3D11ShaderResourceView* testSRV2;
	SimplePixelShader* testPS2;
};

