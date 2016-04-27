#pragma once
#include "PostProcessBase.h"
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Vertex.h"

class Pulse : public PostProcessBase
{
public:
	Pulse(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler);
	virtual SRV* draw(SRV* ppSRV);
	void setDeltaTime(float dt);
	~Pulse();
private:
	// Post process stuff
	RTV* pulseRTV;
	SRV* pulseSRV;
	SRV* pulseTexture;
	SRV* gridTexture;

	float pulseSize = 0;
	float pulseIterator = 0.25f;
	float deltaTime;
	float aspectRatio;

	ID3D11SamplerState* pulseSampler;

	SimplePixelShader* pulsePS;
};

