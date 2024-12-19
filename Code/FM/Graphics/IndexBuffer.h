#pragma once

#include "System\GlobalInclude.h"

#include "IndexBuffer_platform.h"

namespace fm
{
	class IndexBuffer final
	{
		friend class Renderer;
	public:
		IndexBuffer() = default;
		~IndexBuffer();

		bool Create(const uint32 numBytes, const void* const data, bool bDynamic);

		const IndexBufferPlatform* GetPlatformIndexBuffer() const { return mIndexBuffer; }

	private:
		IndexBufferPlatform* mIndexBuffer = nullptr;

		uint32 mNumIndex = 0;
	};
}