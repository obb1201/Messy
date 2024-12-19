#pragma once

#include "System\GlobalInclude.h"

#include "ShaderDefines.h"
#include "PixelShader_platform.h"

namespace fm
{
	class PixelShader final
	{
		friend class Renderer;
	public:
		PixelShader() = default;
		~PixelShader();

		void Create(const void* shaderByteCodes, const uint32 shaderByteCodeSize);

		const PixelShaderPlatform* GetPlatformPixelShader() const { return mPixelShader; }

	private:
		PixelShaderPlatform* mPixelShader = nullptr;
	};
}