//--------------------------------------------------------------------------------------
// File: BasicHLSL11_PS.hlsl
//
// The pixel shader file for the BasicHLSL11 sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

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
	float gAmbient		: packoffset(c0.w);
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
	float3 WorldNormal    : NORMAL;
	//float2 TexCoord  : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(PS_INPUT Input) : SV_TARGET
{
	//float4 abeldo = gDiffuse.Sample(gSamLinear, Input.TexCoord);

	// The light vector aims opposite the direction the light rays travel.
	//float3 lightVec = -gLightDir;

	//float diffuseFactor = saturate(dot(lightVec, Input.Normal));
	//float4 diffuse = diffuseFactor;

	//float4 litColor = abeldo*diffuseFactor;

	//return diffuse;

	return float4(Input.WorldNormal.x, Input.WorldNormal.y, Input.WorldNormal.z, 1.0);
}