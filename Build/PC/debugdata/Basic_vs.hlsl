//--------------------------------------------------------------------------------------
// File: BasicHLSL11_VS.hlsl
//
// The vertex shader file for the BasicHLSL11 sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

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
	//float3 Normal		: NORMAL;
	//float2 TexCoord     : TEXCOORD;
};

struct VS_OUTPUT
{
	float3 WorldNormal  : NORMAL;
	//float2 TexCoord   : TEXCOORD0;
	float4 Position		: SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
	VS_OUTPUT Output;

	// Transform to homogeneous clip space.
	float4 position = mul(float4(Input.Position, 1.0f), gWorld);
	Output.Position = mul(position, gViewProj);

	Output.WorldNormal = float3(1.0f, 1.0f, 1.0f);

	//Output.TexCoord = Input.TexCoord;

	return Output;
}