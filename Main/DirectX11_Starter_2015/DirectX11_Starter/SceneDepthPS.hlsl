
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
	float4 eyeWorldPos = float4(input.view[3].xyz,1);

	float dist = length(eyeWorldPos - input.position)/5000;
	return dist;
}

