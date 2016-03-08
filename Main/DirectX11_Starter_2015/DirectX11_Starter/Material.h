#pragma once
#include <d3d11.h>
#include "Vertex.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include <map>
#include <vector>

class Material
{
protected:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	std::vector<ID3D11ShaderResourceView*> SRVs;
	ID3D11SamplerState * sampler;

	std::map<char*, ID3D11ShaderResourceView*> srvLookup;

public:
	Material(SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader,ID3D11SamplerState * _sampler);
	ID3D11ShaderResourceView * getSRV(char* key);
	ID3D11SamplerState * getSampler();

	void setup();
	void setSRV(char* key, ID3D11ShaderResourceView* in);
	~Material();
};

