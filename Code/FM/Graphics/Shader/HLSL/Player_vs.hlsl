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

cbuffer cbPerSkinned : register(b2)
{
	matrix gBoneTransforms[96];
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

	float3 normalIn = unpackNormal(Input.Normal.xy);
	float3 tangentIn = unpackNormal(Input.Normal.zw);

	// Init array or else we get strange warnings about SV_POSITION.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = Input.Weights.x;
	weights[1] = Input.Weights.y;
	weights[2] = Input.Weights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 positionLocal = float3(0.0f, 0.0f, 0.0f);
	float3 normal = float3(0.0f, 0.0f, 0.0f);
	float3 tangent = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		// Assume no nonuniform scaling when transforming normals, so 
		// that we do not have to use the inverse-transpose.

		positionLocal += weights[i] * mul(float4(Input.Position, 1.0f), gBoneTransforms[Input.BoneIndices[i]]).xyz;
		normal += weights[i] * mul(normalIn, (float3x3)gBoneTransforms[Input.BoneIndices[i]]);
		tangent += weights[i] * mul(tangentIn, (float3x3)gBoneTransforms[Input.BoneIndices[i]]);
	}

	// Transform to homogeneous clip space.
	float4 positionWorld = mul(float4(positionLocal, 1.0f), gWorld);
	Output.Position = mul(positionWorld, gViewProj);

	Output.WorldNormal = mul(normal, (float3x3)gWorldInvTranspose);
	Output.WorldTangent = mul(tangent, (float3x3)gWorld);

	Output.TexCoord = Input.TexCoord;

	return Output;
}