
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


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1,0,0,1);
	return pixels.Sample(trilinear, input.uv);

	//return float4(1,1,0.5f,1);
	float4 totalColor = float4(0,0,0,0);
	uint sampleCount = 0;

	for (int y = -blurAmount; y <= blurAmount; y++)
	{
		for (int x = -blurAmount; x <= blurAmount; x++)
		{
			float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);
			uv.x = saturate(uv.x);
			uv.y = saturate(uv.y);
			totalColor += pixels.Sample(trilinear, uv);

			sampleCount++;
		}
	}

	return totalColor / sampleCount;
}