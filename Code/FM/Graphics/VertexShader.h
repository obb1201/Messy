#pragma once

#include "System\GlobalInclude.h"

#include "ShaderDefines.h"
#include "VertexShader_platform.h"

namespace fm
{
	class VertexShader final
	{
		friend class Renderer;
	public:
		VertexShader() = default;
		~VertexShader();

		void Create(const void* shaderByteCodes, const uint32 shaderByteCodeSize);

		const VertexShaderPlatform* GetPlatformVertexShader() const { return mVertexShader; }

	private:
		VertexShaderPlatform* mVertexShader = nullptr;
	};
}