#pragma once
#include "PostProcessBase.h"
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Vertex.h"
class Blur: public PostProcessBase
{
public:
	Blur(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext,ID3D11SamplerState* _sampler, ID3D11RenderTargetView* _rtv);
	virtual ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV);
	~Blur();
private:
	float windowWidth, windowHeight;

	// Post process stuff
	ID3D11RenderTargetView* blurRTV;
	ID3D11ShaderResourceView* blurSRV;
	SimpleVertexShader* ppVS;
	SimplePixelShader* ppPS;

	ID3D11Device* device;
	ID3D11DeviceContext * deviceContext;
	ID3D11SamplerState* sampler;
};

