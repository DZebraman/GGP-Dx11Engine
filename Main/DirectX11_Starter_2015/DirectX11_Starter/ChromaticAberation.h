#pragma once
#include "PostProcessBase.h"

class ChromaticAberation : public PostProcessBase
{
public:
	ChromaticAberation(int width, int height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler);
	virtual SRV* draw(SRV* ppSRV);
	~ChromaticAberation();
private:
	float amount;

	SimplePixelShader* caPS;

	SRV* caSRV;
	RTV* caRTV;
};

