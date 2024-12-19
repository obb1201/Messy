#pragma once

#include "VertexInputLayout_platform.h"

#include "System\GlobalInclude.h"

namespace fm
{
	class VertexInputLayout final
	{
		friend class Renderer;
	public:
		enum class eInputLayout
		{
			Static,
			Skinned,
			UI,

			Count,
		};

		VertexInputLayout() = default;
		~VertexInputLayout();

		void Create(const eInputLayout layoutType, const void* shaderByteCodes, const uint32 shaderByteCodeSize);

		const VertexInputLayoutPlatform* GetPlatformInputLayout() const { return mInputLayout; }

	private:
		VertexInputLayoutPlatform* mInputLayout = nullptr;
	};
}