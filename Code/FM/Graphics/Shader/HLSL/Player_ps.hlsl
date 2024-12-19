//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//cbuffer cbPerObject : register(b0)
//{
//	float4 g_vObjectColor : packoffset(c0);
//};

cbuffer cbPerFrame : register(b0)
{
	float3 gLightDir	: packoffset(c0);
	float gAmbient : packoffset(c0.w);
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
//Texture2D    gDiffuse	: register(t0);
//SamplerState gSamLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
	float3 WorldNormal  : NORMAL;
	float3 WorldTangent	: TANGENT;
	float2 TexCoord		: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(PS_INPUT Input) : SV_TARGET
{
	//float4 abeldo = gDiffuse.Sample(gSamLinear, Input.TexCoord);

	Input.WorldNormal = normalize(Input.WorldNormal);

	float3 lightVec = -gLightDir;
	float diffuseFactor = dot(lightVec, Input.WorldNormal);

	return float4(diffuseFactor.xxx, 1.0f);
}