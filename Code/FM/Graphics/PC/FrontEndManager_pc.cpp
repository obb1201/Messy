//TODO(ryan) : HACK : will be eliminated with d3dcompiler.lib after Rendering Architecture is completed.
#include <d3dcompiler.h>

#include "Graphics/FrontEndManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Mesh.h"

#include "Resource/Data/CompressedVertexData.h"

#include "System/Assert.h"
#include "System/Logger.h"
#include "System/Memory.h"

#include "Resource/Data/IndexData.h"
#include "System/Container/Vector.h"

// headers of the compiled shader
#include "Graphics/Shader/UI_vs.h"
#include "Graphics/Shader/UI_ps.h"


namespace fm
{
	FrontEndManager::~FrontEndManager()
	{
		SafeRelease(mRasterizerState);
	}

	void FrontEndManager::Init()
	{
		// --- Create Vertex buffer
		SimpleVertex vertices[4];
		vertices[0] = { -1.f, 1.f };
		vertices[1] = { 1.f, 1.f };
		vertices[2] = { -1.f, -1.f };
		vertices[3] = { 1.f, -1.f };

		mVertexBuffer.Create(sizeof(vertices), &vertices, false);

		// --- Create Index buffer
		uint16 indices[] =
		{
			0,1,2, 2,1,3
		};
		mIndexBuffer.Create(sizeof(indices), indices, false);

		//
		mStride = sizeof(SimpleVertex);
		mOffset = 0;

		//
		buildShader();
		buildRenderState();

		// --- Create Constant buffer
		mVertexConstantBuffer.Create(sizeof(UIVertextConstantBuffer));
		mPixelConstantBuffer.Create(sizeof(UIPixelConstantBuffer));
	}

	void FrontEndManager::buildShader()
	{
		Renderer& renderer = Renderer::GetInstance();

		VertexInputLayout::eInputLayout layoutType = VertexInputLayout::eInputLayout::UI;

		mInputLayout.Create(layoutType, UI_vs_bytecodes, sizeof(UI_vs_bytecodes));
		mVertexShader.Create(UI_vs_bytecodes, sizeof(UI_vs_bytecodes));
		mPixelShader.Create(UI_ps_bytecodes, sizeof(UI_ps_bytecodes));
	}

	void FrontEndManager::buildRenderState()
	{
		Renderer& renderer = Renderer::GetInstance();

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		// 	rasterizerDesc.DepthBias = 100000;
		// 	rasterizerDesc.DepthBiasClamp = 0.0f;
		// 	rasterizerDesc.SlopeScaledDepthBias = 1.0f;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;

		Verify(S_OK == renderer.GetDevice().CreateRasterizerState(&rasterizerDesc, &mRasterizerState));
	}
}