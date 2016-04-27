// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

cbuffer Data : register(c5) {
	float uvScale;
}

// Textures and such
Texture2D pixels			: register(t0);
Texture2D pulse				: register(t9);
SamplerState trilinear		: register(s0);
SamplerState textureClamp	: register(s1);


float4 main(VertexToPixel input) : SV_TARGET
{
	float4 pixelsColor = pixels.Sample(trilinear,input.uv,0);
	float4 pulseColor = pulse.Sample(textureClamp, (input.uv * uvScale), 0);
	return pixelsColor * pulseColor;
}