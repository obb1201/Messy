#pragma once

#include "System/Globalinclude.h"
#include "System/Hash.h"

#include "Graphics/Indexbuffer.h"
#include "Graphics/Vertexbuffer.h"
#include "Graphics/ConstantBuffer.h"

#include "Graphics/VertexInputLayout.h"

#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"

#include "Graphics/RenderState.h"
#include "Graphics/ShaderReourceView.h"

#include "System/Math/Matrix.h"
#include "System/Math/Vector4.h"

namespace fm
{
	struct SimpleVertex
	{
		float x;
		float y;
	};

	struct UIVertextConstantBuffer
	{
		Matrix Transformation;
	};

	struct UIPixelConstantBuffer
	{
		Vector4 Color;
	};

	class FrontEndManagerPC
	{
	public:

	protected:
		VertexBuffer mVertexBuffer;
		IndexBuffer mIndexBuffer;

		VertexInputLayout mInputLayout;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		RasterizerState* mRasterizerState = nullptr;

		ConstantBuffer mVertexConstantBuffer;
		ConstantBuffer mPixelConstantBuffer;

		uint32 mStride = 0;
		uint32 mOffset = 0;
	};
}