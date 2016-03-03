
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
	float3 tan			: TAN;
	float3 biTan		: BITAN;
};

struct DirectionalLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

#define numLights 2

Texture2D normalTexture    : register(t0);
Texture2D specTexture	   : register(t1);
Texture2D diffTexture	   : register(t2);
SamplerState trilinear     : register(s0);

cbuffer lightBuffer : register(b0) {
	DirectionalLight light1;
	DirectionalLight light2;
}

float getLightAmount(DirectionalLight light, float3 normal) : SV_TARGET
{
	float3 negLightDir = -normalize(light.Direction);
	return saturate(dot(normal, negLightDir));
}

float fresnel(float3 fwd, float3 normal, int exp, float intensity) {
	return saturate(pow(1-dot(normalize(-fwd), normal),exp)*intensity);
}


#define celDivisions 2

float celShade(float input) {
	return ceil(input * celDivisions) / celDivisions;
}

//From http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
float4 rgb2hsv(float4 c)
{
	float4 K = float4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	float4 p = lerp(float4(c.bg, K.wz), float4(c.gb, K.xy), step(c.b, c.g));
	float4 q = lerp(float4(p.xyw, c.r), float4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return float4(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x,1);
}

//From http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
float4 hsv2rgb(float4 c)
{
	float4 K = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	float4 p = abs(frac(c.xxxx + K.xyzx) * 6.0 - K.wwww);
	return float4(c.z * lerp(K.xxx, saturate(p - K.xxx), c.y),1);
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------

float4 main(VertexToPixel input) : SV_TARGET
{
	float3 bitan = cross(input.tan,input.normal);
	float3 tan = cross(input.biTan, input.normal);

	float3 fwd = float3(input.view[0][2], input.view[1][2], input.view[2][2]);
	float3 eyeWorldPos = float3(input.view[0][3], input.view[1][3], input.view[2][3]);
	//float3 eyeWorldPos = input.view[3];

	float4 specColor = float4(0,0,0,1);


	float4 objColor = float4(0.6, 1, 0.8, 1);

	float4 nrmTex = normalTexture.Sample(trilinear, input.uv);
	nrmTex = 2 * nrmTex - 1;
	//nrmTex.y *= -1;

	float3x3 TBN = float3x3(input.tan, bitan, input.normal);

	float3 nrm = mul(nrmTex, TBN);
	nrm.y *= -1;

	float3 halfAngle = normalize(normalize(eyeWorldPos - input.position) - light1.Direction);

	float3 LightReflect = normalize(reflect(light1.Direction, input.normal));
	float SpecularFactor1 = dot(nrm, halfAngle);
	//LightReflect = normalize(reflect(light2.Direction, input.normal));
	float SpecularFactor2 = dot(input.normal -eyeWorldPos, LightReflect);

	if (SpecularFactor1 > 0) {
		SpecularFactor1 = pow(SpecularFactor1, 8);
		specColor += float4(light1.DiffuseColor * 16 * SpecularFactor1);
	}/*if (SpecularFactor2 > 0) {
		SpecularFactor2 = pow(SpecularFactor2, 64);
		specColor += float4(light2.DiffuseColor * 1 * SpecularFactor2);
	}*/

	float4 specTex = specTexture.Sample(trilinear, input.uv);
	float4 diffTex = diffTexture.Sample(trilinear, input.uv);

	specColor = saturate(specColor);
	//return saturate(specColor * specTex);

	/*float lightAmount1 = getLightAmount(light1, nrm);
	float lightAmount2 = getLightAmount(light2, nrm);*/
	float lightAmount1 = getLightAmount(light1, nrm);
	float lightAmount2 = getLightAmount(light2, nrm);

	float4 ambColor = light1.AmbientColor + light2.AmbientColor;

	//cel shading
	/*lightAmount1 = celShade(lightAmount1);
	lightAmount2 = celShade(lightAmount2);*/

	float fresnelAmount = fresnel(fwd, nrm, 12.f,0.1f);

	float4 inHColor = rgb2hsv(diffTex);
	inHColor.x *= -1;
	inHColor = hsv2rgb(inHColor);

	//return inHColor * specColor*pow(specTex, 2);

	//return fresnelAmount;
	return saturate((light1.DiffuseColor*lightAmount1 * diffTex) + ambColor + (fresnelAmount*specColor) + (specColor*pow(specTex,2) * inHColor));
	return  float4(input.normal,1);
}

