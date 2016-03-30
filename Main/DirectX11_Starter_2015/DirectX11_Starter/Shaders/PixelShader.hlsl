
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 worldPos		: WORLDPOS;
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
Texture2D glossTexture	   : register(t3);
TextureCube cubeMap : register(t4);
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

	float2 uvScale = input.uv * 1;

	float3 fwd = float3(input.view[0][2], input.view[1][2], input.view[2][2]);
	//float3 eyeWorldPos = float3(input.view[0][3], input.view[1][3], input.view[2][3]);
	float3 eyeWorldPos = input.view[3];

	float4 specColor = float4(0,0,0,1);

	float4 objColor = float4(0.6, 1, 0.8, 1);

	float4 glossTex = glossTexture.Sample(trilinear, uvScale);

	float4 nrmTex = normalTexture.Sample(trilinear, uvScale);
	nrmTex = 2 * nrmTex - 1;
	//nrmTex.y *= -1;

	float3x3 TBN = float3x3(input.tan, bitan, input.normal);

	float3 nrm = mul(nrmTex, TBN);
	nrm = lerp(input.normal, nrm, 0.75f);
	//nrm.y *= -1;

	float3 halfAngle = -normalize(fwd + light1.Direction);

	float4 reflectionColor = cubeMap.Sample(
		trilinear,
		reflect(-normalize(eyeWorldPos - input.worldPos), input.normal));

	float3 LightReflect = normalize(reflect(light1.Direction, nrm));
	float SpecularFactor1 = dot(lerp(input.normal,nrm,0.6f), halfAngle);
	float SpecularFactor2 = dot(input.normal -eyeWorldPos, halfAngle);

	if (SpecularFactor1 > 0) {
		SpecularFactor1 = pow(SpecularFactor1, lerp(2, 32, glossTex.x));
		specColor += float4(light1.DiffuseColor * 1 * SpecularFactor1);
	}if (SpecularFactor2 > 0) {
		SpecularFactor2 = pow(SpecularFactor1, lerp(2, 32,glossTex.x));
		specColor += float4(light2.DiffuseColor * 1 * SpecularFactor2);
	}

	float4 specTex = specTexture.Sample(trilinear, uvScale);

	float4 diffTex = diffTexture.Sample(trilinear, uvScale);

	//return (SpecularFactor1 * 1 * specTex);

	/*float lightAmount1 = getLightAmount(light1, nrm);
	float lightAmount2 = getLightAmount(light2, nrm);*/
	float lightAmount1 = getLightAmount(light1, nrm);
	float lightAmount2 = getLightAmount(light2, nrm);

	float4 ambColor = light1.AmbientColor + light2.AmbientColor;

	//cel shading
	/*lightAmount1 = celShade(lightAmount1);
	lightAmount2 = celShade(lightAmount2);*/

	float fresnelAmount = fresnel(fwd, nrm, 4.f,0.5f);

	//return inHColor * specColor*pow(specTex, 2);
	return reflectionColor;

	//return fresnelAmount;
	return ((light1.DiffuseColor*lightAmount1 + light2.DiffuseColor*lightAmount2) * diffTex)
		+ ambColor
		+ (fresnelAmount * reflectionColor)
		+ (SpecularFactor1 * lerp(0.75f,2.f,glossTex)* specTex * reflectionColor);
	return  float4(input.normal,1);
}

