
cbuffer Data : register(b0)
{
	float pixelWidth;
	float pixelHeight;
	int blurAmount;
}


// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D pixels		: register(t0);
SamplerState trilinear	: register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1,0,0,1);
	return pixels.Sample(trilinear,input.uv) + float4(0.2,0,0,1);
}