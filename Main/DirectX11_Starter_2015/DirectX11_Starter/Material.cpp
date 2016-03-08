#include "Material.h"



Material::Material(SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, ID3D11SamplerState * _sampler)
{
	vertexShader = _vertexShader;
	pixelShader = _pixelShader;
	sampler = _sampler;
}

void Material::setSRV(char* key, ID3D11ShaderResourceView* in) {
	if (!srvLookup[key]) {
		SRVs.push_back(in);
		srvLookup[key] = in;
	}
}

void Material::setup() {
	for (auto iter = srvLookup.begin(); iter != srvLookup.end(); iter++) {
		pixelShader->SetShaderResourceView(iter->first, iter->second);
	}
	pixelShader->SetSamplerState("trilinear", sampler);
}

ID3D11SamplerState * Material::getSampler() { return sampler; }
ID3D11ShaderResourceView * Material::getSRV(char* key) { return srvLookup[key]; }

Material::~Material()
{
}
