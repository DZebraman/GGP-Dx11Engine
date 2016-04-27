// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

cbuffer Data : register(c5) {
	float uvScale;
	float aspectRatio;
	float offSetX;
}

// Textures and such
Texture2D pixels			: register(t0);
Texture2D pulse				: register(t1);
Texture2D grid				: register(t2);

SamplerState trilinear		: register(s0);
SamplerState textureClamp	: register(s1);


float4 main(VertexToPixel input) : SV_TARGET
{
	float4 pixelsColor = pixels.Sample(trilinear,input.uv,0);
	float4 pulseColor = pulse.Sample(trilinear, input.uv + float2(-offSetX,0), 0);

	float uvS = lerp(50, 52, pulseColor.x);
	float2 uvOff = float2(uvS.xx);
	//float2 uvOff = float2(80, 80);

	float4 gridColor = grid.Sample(trilinear, input.uv * uvOff ,0);
	//return pulseColor.xxxx * (gridColor.xxxx + 0.125f) + 0.125f;
	return saturate((pixelsColor + gridColor.xxxx) * (pow(pulseColor.xxxx,4) +0.05f));
}