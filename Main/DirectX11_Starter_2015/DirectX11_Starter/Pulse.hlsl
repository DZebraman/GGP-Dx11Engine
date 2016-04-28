// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

cbuffer Data : register(c0) {
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
	float4 pulseColor = pulse.Sample(trilinear, (input.uv * 5) + float2(-offSetX,0), 0);

	//float uvS = lerp(60, 59, pulseColor.x);
	float2 uvOff = float2(60,60);
	//float2 uvOff = float2(80, 80);

	float4 gridColor = grid.Sample(trilinear, input.uv * uvOff ,0);
	gridColor.y = gridColor.x;
	gridColor.z = gridColor.x;

	gridColor *= float4(0, 1, 1, 1);

	//return (pow(pulseColor.xxxx, 0.75f)) * (gridColor);
	//return (pow(pulseColor.xxxx, 4.f)) * pixelsColor;
	float4 pulseLight = pow(pulseColor.xxxx, 4.f);
	return saturate(pixelsColor*(pulseLight+0.06f) + gridColor*(pulseLight + 0.25f));
}