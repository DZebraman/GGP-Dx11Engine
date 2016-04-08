#pragma once
#include "DirectXGameCore.h"
#include "PostProcessBase.h"
#include "SimpleShader.h"
class Tint : public PostProcessBase
{
public:
	Tint(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler, ID3D11RenderTargetView* _rtv);
	~Tint();
	virtual ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV);
private:
	// Post process stuff
	ID3D11RenderTargetView* tintRTV;
	ID3D11ShaderResourceView* tintSRV;
	SimpleVertexShader* ppVS;
	SimplePixelShader* ppPS;
};

