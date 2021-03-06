#pragma once
#include "PostProcessBase.h"

class EdgeDetect : public PostProcessBase
{
public:
	EdgeDetect(int width, int height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext);
	virtual SRV* draw(SRV* ppSRV);
	void setBlur(bool _blur);
	void setSamples(float _samples);
	void setOutlineWidth(float _width);
	~EdgeDetect();
private:
	// Post process stuff
	RTV* edgeRTV;
	SRV* edgeSRV;

	RTV* blurRTV;
	SRV* blurSRV;

	bool blur;
	float blurSamples;
	float outlineWidth;

	SimplePixelShader* edgePS;
	SimplePixelShader* blurPS;
};

