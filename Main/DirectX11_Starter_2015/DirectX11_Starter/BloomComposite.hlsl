
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
Texture2D bright		: register(t1);
Texture2D test		    : register(t6);
SamplerState trilinear	: register(s0);


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(1,1,1,1);
	//return pixels.Sample(trilinear, input.uv);
	//return test.Sample(trilinear, input.uv);

	//return float4(1,1,0.5f,1);
	float4 totalColorX = float4(0,0,0,1);
	float4 totalColorY = float4(0, 0, 0, 1);
	uint sampleCount = 2* blurAmount + 1;
	
	totalColorY += test.Sample(trilinear, input.uv);
	sampleCount++;

	for (int i = 1; i <= blurAmount; i++) {
		float2 uvMod = float2(0, i * pixelHeight);

		totalColorY += test.Sample(trilinear, input.uv + uvMod);
		totalColorY += test.Sample(trilinear, input.uv - uvMod);
	}
	for (int i = 1; i <= blurAmount; i++) {

		float2 uvMod = float2(i * pixelHeight, 0);

		totalColorX += test.Sample(trilinear, input.uv + uvMod);
		totalColorX += test.Sample(trilinear, input.uv - uvMod);
	}

	/*for (int y = -blurAmount; y <= blurAmount; y++)
	{
		for (int x = -blurAmount; x <= blurAmount; x++)
		{
			float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);
			uv.x = saturate(uv.x);
			uv.y = saturate(uv.y);
			totalColor += test.Sample(trilinear, uv);

			sampleCount++;
		}
	}*/

	return ((totalColorY+totalColorX) / sampleCount) + pixels.Sample(trilinear, input.uv);
}