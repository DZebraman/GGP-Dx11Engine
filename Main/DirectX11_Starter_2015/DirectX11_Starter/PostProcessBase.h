#pragma once
#include "DirectXGameCore.h"

typedef ID3D11ShaderResourceView SRV;

class PostProcessBase
{
public:
	PostProcessBase();
	~PostProcessBase();
	virtual SRV* draw(SRV* ppSRV);
protected:
	float windowWidth, windowHeight;

	void setupRenderTarget(ID3D11RenderTargetView** rtv, SRV** srv);

	ID3D11Device* device;
	ID3D11DeviceContext * deviceContext;
	ID3D11SamplerState* sampler;
};

