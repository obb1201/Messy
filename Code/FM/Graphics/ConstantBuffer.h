#pragma once

#include "ConstantBuffer_platform.h"

namespace fm
{
	class ConstantBuffer final
	{
		friend class Renderer;
	public:
		// --- Methods
		ConstantBuffer() = default;
		~ConstantBuffer();

		void Create(const uint32 numBytes);

	private:
		ConstantBufferPlatform* mConstantBuffer;
	};
}
