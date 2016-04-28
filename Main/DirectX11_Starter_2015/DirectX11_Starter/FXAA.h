#pragma once
#include "PostProcessBase.h"
#include "EdgeDetect.h"

class FXAA : public PostProcessBase
{
public:
	FXAA(int width, int height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler);
	virtual SRV* draw(SRV* ppSRV);
	~FXAA();
private:
	EdgeDetect* edges;

	SimplePixelShader* fxaaPS;

	RTV* aaRTV;
	SRV* aaSRV;
};

