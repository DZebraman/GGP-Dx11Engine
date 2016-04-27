
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	matrix view			: VIEW;
	matrix world		: WORLD;
	float2 uv			: UV;
};

struct DirectionalLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer lightBuffer : register(b0) {
	DirectionalLight light1;
}

#define numLights 1

Texture2D diffTexture	   : register(t1);
SamplerState trilinear     : register(s0);

float getLightAmount(DirectionalLight light, float3 normal) : SV_TARGET
{
	float3 negLightDir = -normalize(light.Direction);
	return saturate(dot(normal, negLightDir));
}

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1,1,1,1);
	return diffTexture.Sample(trilinear, input.uv) * getLightAmount(light1,input.normal);
}

