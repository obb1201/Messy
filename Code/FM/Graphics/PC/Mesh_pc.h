#pragma once

#include "System/Globalinclude.h"
#include "System/Hash.h"

#include "Graphics/Indexbuffer.h"
#include "Graphics/Vertexbuffer.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/ShaderReourceView.h"
#include "Graphics/RenderState.h"
#include "Graphics/VertexInputLayout.h"

#include "System/Math/Vector3.h"
#include "System/Math/Vector2.h"
#include "System/Math/Matrix.h"

#include "Animation/AnimationDefine.h"

namespace fm
{
	static const uint32 MAX_NUMSTREAMS = 2;

	struct PerObjectConstantBuffer
	{
		Matrix WorldTranspose;
		Matrix WorldInvertTranspose;
	};

	struct PerCameraConstantBuffer
	{
		Matrix ViewProjectionTranspose;
	};

	struct PerSkinnedConstantBuffer
	{
		Matrix BoneTransform[MAXIMUM_BONES];
	};

	struct PerFrameConstantBuffer
	{
		Vector3 LightDirection;
		float	Ambient;
	};

	class MeshPC
	{
	public:

	protected:
		VertexBuffer mVBStream[MAX_NUMSTREAMS];
		IndexBuffer mIndexBuffer;

		VertexInputLayout mInputLayout;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		ConstantBuffer mCBPerObject;
		ConstantBuffer mCBPerFrame;

		ShaderResourceViewPlatform* mShaderResourceView = nullptr;
		SamplerStatePlatform* mSamplerState = nullptr;

		RasterizerState* mRasterizerState = nullptr;
		BlendState* mBlendState = nullptr;
		DepthStencilState* mDepthStencilState = nullptr;

		uint32 mStride[MAX_NUMSTREAMS] = { 0, };
		uint32 mOffset[MAX_NUMSTREAMS] = { 0, };
	};
}