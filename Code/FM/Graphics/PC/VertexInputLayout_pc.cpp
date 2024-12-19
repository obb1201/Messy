#include "System/Memory.h"
#include "System/Assert.h"

#include "Graphics/VertexInputLayout.h"
#include "Graphics/Renderer.h"

namespace fm
{
	struct InputLayouInfo
	{
		static const uint8 MAX_ELEMENTCOUNT = 6;

		uint32 ElementCount;
		D3D11_INPUT_ELEMENT_DESC InputElementDescriptions[MAX_ELEMENTCOUNT];
	};

	InputLayouInfo InputLayoutInfoList[(uint32)VertexInputLayout::eInputLayout::Count]
	{
		{ 5, {	{ "POSITION", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BONEINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 1, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "WEIGHTS", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			 } 
		},
		{ 1, {	{ "POSITION", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		     } 
		},
		// TODO(Rachel) : If use DXGI_FORMAT_R32G32B32_FLOAT, rectangle is not displayed (only triangle)
		//				: If use DXGI_FORMAT_R32G32_FLOAT, rectangle is displayed. What difference bewtween...?
		{ 2, { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
			   { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
			 } 
		},
	};

	VertexInputLayout::~VertexInputLayout()
	{
		SafeRelease(mInputLayout);
	}

	void VertexInputLayout::Create(const eInputLayout layoutType, const void* shaderByteCodes, const uint32 shaderByteCodeSize)
	{
		Renderer& renderer = Renderer::GetInstance();

		Verify(S_OK == renderer.GetDevice().CreateInputLayout(InputLayoutInfoList[(uint32)layoutType].InputElementDescriptions,
			InputLayoutInfoList[(uint32)layoutType].ElementCount, shaderByteCodes, shaderByteCodeSize, &mInputLayout));
	}
}