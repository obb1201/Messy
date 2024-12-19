//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
cbuffer cbPerCamera : register(b0)
{
	matrix gViewProj		: packoffset(c0);
};

cbuffer cbPerObject : register(b1)
{
	matrix gWorld				: packoffset(c0);
	matrix gWorldInvTranspose	: packoffset(c4);
};
//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Position		: POSITION;
	float2 TexCoord     : TEXCOORD;
	float4 Normal		: NORMAL;
	uint4 BoneIndices	: BONEINDICES;
	float4 Weights		: WEIGHTS;
};

struct VS_OUTPUT
{
	float3 WorldNormal  : NORMAL;
	float3 WorldTangent : TANGENT;
	float2 TexCoord		: TEXCOORD0;
	float4 Position		: SV_POSITION;
};
//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------
float signZ(float value)
{
	return int(value * 32767.0f) & 0x00000001 == 1 ? 1.0f : -1.0f;
}

float3 unpackNormal(float2 packedValue)
{
	float3 result;
	result.xy = packedValue.xy;
	result.z = sqrt(1.0f - dot(packedValue.xy, packedValue.xy)) * signZ(packedValue.x);

	return result;
}
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
	VS_OUTPUT Output;

	// Transform to homogeneous clip space.
	float4 position = mul(float4(Input.Position, 1.0f), gWorld);
	Output.Position = mul(position, gViewProj);

	float3 normal = unpackNormal(Input.Normal.xy);
	Output.WorldNormal = mul(normal, (float3x3)gWorldInvTranspose);

	float3 tangent = unpackNormal(Input.Normal.zw);
	Output.WorldTangent = mul(tangent, (float3x3)gWorld);

	Output.TexCoord = Input.TexCoord;

	return Output;
}