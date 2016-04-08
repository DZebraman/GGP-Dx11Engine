#pragma once
#include "PostProcessBase.h"
#include "SimpleShader.h"
class EdgeDetect : public PostProcessBase
{
public:
	EdgeDetect(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext);
	virtual SRV* draw(SRV* ppSRV);
	~EdgeDetect();
private:
	// Post process stuff
	RTV* edgeRTV;
	SRV* edgeSRV;

	RTV* blurRTV;
	SRV* blurSRV;

	SimplePixelShader* edgePS;
	SimplePixelShader* blurPS;
};

