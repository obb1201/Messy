#include "Graphics/Mesh.h"
#include "Graphics/Renderer.h"

// headers of the compiled shader
#include "Graphics/Shader/Basic_vs.h"
#include "Graphics/Shader/Basic_ps.h"
#include "Graphics/Shader/Player_vs.h"
#include "Graphics/Shader/Player_ps.h"

#include "Resource/Data/CompressedVertexData.h"

#include "System/Assert.h"
#include "System/Logger.h"
#include "System/Memory.h"

namespace fm
{
#define SHADERTYPE_LISTENTRY(name) name##_vs_bytecodes
 	static const uint8* VertexShaderByteCodes[] 
	{
 		SHADERTYPE_LIST
 	};
#undef SHADERTYPE_LISTENTRY

#define SHADERTYPE_LISTENTRY(name) sizeof(name##_vs_bytecodes)
	static uint32 VertexShaderByteCodeSize[]
	{
		SHADERTYPE_LIST
	};
#undef SHADERTYPE_LISTENTRY

#define SHADERTYPE_LISTENTRY(name) name##_ps_bytecodes
	static const uint8* PixelShaderByteCodes[]
	{
		SHADERTYPE_LIST
	};
#undef SHADERTYPE_LISTENTRY

#define SHADERTYPE_LISTENTRY(name) sizeof(name##_ps_bytecodes)
	static uint32 PixelShaderByteCodeSize[]
	{
		SHADERTYPE_LIST
	};
#undef SHADERTYPE_LISTENTRY

	static const uint32 VertexStride[] = { sizeof(CompressedVertexData::Stream0), sizeof(CompressedVertexData::Stream1), };

	Mesh::Mesh(const void* const vertexData0, const void* const vertexData1, const void* const indexData, const eMeshType meshType, 
		const uint8 numMeshParts, const uint32 numVertices, const uint32 numIndices, 
		const uint32 vertexSize0, const uint32 vertexSize1, const uint32 indexSize)
		: mMeshType(meshType)
		, mNumMeshparts(numMeshParts)
		, mNumVertices(numVertices)
		, mNumIndices(numIndices)
	{
		mIndexBuffer.Create(indexSize, indexData, mbDynamic);
		mVBStream[0].Create(vertexSize0, vertexData0, mbDynamic);
		mVBStream[1].Create(vertexSize1, vertexData1, mbDynamic);

		mStride[0] = VertexStride[0];
		mOffset[0] = 0;
		mStride[1] = VertexStride[1];
		mOffset[1] = 0;

		mCBPerObject.Create(sizeof(PerObjectConstantBuffer));
		mCBPerFrame.Create(sizeof(PerFrameConstantBuffer));

		// HACK : where do they place?
		eShaderType shaderType = eShaderType::Basic;
		VertexInputLayout::eInputLayout layoutType = VertexInputLayout::eInputLayout::Static;

		// TODO : shaders can be shared with each mesh?
		mInputLayout.Create(layoutType, VertexShaderByteCodes[(uint32)shaderType], VertexShaderByteCodeSize[(uint32)shaderType]);
		mVertexShader.Create(VertexShaderByteCodes[(uint32)shaderType], VertexShaderByteCodeSize[(uint32)shaderType]);
		mPixelShader.Create(PixelShaderByteCodes[(uint32)shaderType], PixelShaderByteCodeSize[(uint32)shaderType]);

		// HACK
		buildRenderState();
	}

	Mesh::~Mesh()
	{
		SafeRelease(mShaderResourceView);
		SafeRelease(mSamplerState);

		SafeRelease(mRasterizerState);
		SafeRelease(mBlendState);
		SafeRelease(mDepthStencilState);
	}

	void Mesh::buildRenderState()
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
