#pragma once
#include "PostProcessBase.h"
#include "SimpleShader.h"
#include "Camera.h"
#include "Entity.h"
class ScreenDepth : public PostProcessBase
{
public:
	ScreenDepth(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler, Camera* _camera, Entity** _entities, int _numMeshes);
	virtual ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV);
	~ScreenDepth();
private:
	Camera* camera;

	Entity** entities;
	int numMeshes;

	// Post process stuff
	ID3D11RenderTargetView* depthRTV;
	ID3D11ShaderResourceView* depthSRV;

	SimplePixelShader* depthPS;
	SimpleVertexShader* depthVS;
};

