//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
cbuffer cbPerUITran : register(b1)
{
	matrix gTransformation;
};

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float2 Position		: POSITION;
	// TODO: Get Texture
	//float2 TexCoord        : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION;
	// float2 TexCoord     : TEXCOORD0;
};	

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
	VS_OUTPUT Output;

	// TODO: apply scale and position
	float4 position = mul(gTransformation, float4(Input.Position, 0.1f, 1.f)); 
	
	Output.Position = position;
	
	// TODO: use proper texcoord from veterxbuffer
	//Output.TexCoord = float2(0, 0);
	
	return Output;
}
