#pragma once
#include "PostProcessBase.h"
#include "SimpleShader.h"
#include "Vertex.h"
class Bloom : public PostProcessBase
{
public:
	Bloom(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler);
	~Bloom();
	virtual ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV, ID3D11DepthStencilView* depthStencilView);
private:

	float windowWidth, windowHeight;

	// Post process stuff
	ID3D11RenderTargetView* bloomFilterTarget;
	ID3D11ShaderResourceView* bloomFilterSRV;

	ID3D11RenderTargetView* compositeTarget;
	ID3D11ShaderResourceView* compositeSRV;

	SimpleVertexShader* bloomVS;
	SimplePixelShader* filterPS;
	SimplePixelShader* compositePS;

	ID3D11Device* device;
	ID3D11DeviceContext * deviceContext;
	ID3D11SamplerState* sampler;
};

