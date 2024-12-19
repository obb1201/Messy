#pragma once

#include "System\GlobalInclude.h"

#include "VertexBuffer_platform.h"

namespace fm
{
	class VertexBuffer final
	{
		friend class Renderer;
	public:
		VertexBuffer() = default;
		~VertexBuffer();

		bool Create(const uint32 numBytes, const void* const data, bool bDynamic);

		const VertexBufferPlatform* GetPlatformVertexBuffer() const { return mVertexBuffer; }

	private:
		VertexBufferPlatform* mVertexBuffer = nullptr;

		uint32 mNumVertex = 0;
	};
}