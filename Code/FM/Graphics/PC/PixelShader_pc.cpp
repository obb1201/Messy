#include "System/Memory.h"
#include "System/Assert.h"

#include "Graphics/PixelShader.h"
#include "Graphics/Renderer.h"

namespace fm
{
	PixelShader::~PixelShader()
	{
		SafeRelease(mPixelShader);
	}

	void PixelShader::Create(const void* shaderByteCodes, const uint32 shaderByteCodeSize)
	{
		Renderer& renderer = Renderer::GetInstance();

		Verify(S_OK == renderer.GetDevice().CreatePixelShader(shaderByteCodes, shaderByteCodeSize, NULL, &mPixelShader));
	}
}