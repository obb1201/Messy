#include "System/Memory.h"
#include "System/Assert.h"

#include "Graphics/VertexShader.h"
#include "Graphics/Renderer.h"

namespace fm
{
	VertexShader::~VertexShader()
	{
		SafeRelease(mVertexShader);
	}

	void VertexShader::Create(const void* shaderByteCodes, const uint32 shaderByteCodeSize)
	{
		Renderer& renderer = Renderer::GetInstance();

		Verify(S_OK == renderer.GetDevice().CreateVertexShader(shaderByteCodes, shaderByteCodeSize, nullptr, &mVertexShader));
	}
}