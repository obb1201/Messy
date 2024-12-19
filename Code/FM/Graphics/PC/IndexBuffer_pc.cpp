#include "System/Memory.h"

#include "Graphics/IndexBuffer.h"
#include "Graphics/Renderer.h"

namespace fm
{
	IndexBuffer::~IndexBuffer()
	{
		SafeRelease(mIndexBuffer);
	}

	bool IndexBuffer::Create(const uint32 numBytes, const void* const data, bool bDynamic)
	{
		Renderer& renderer = Renderer::GetInstance();

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = bDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = numBytes;
		bufferDesc.CPUAccessFlags = bDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;

		Verify(S_OK == renderer.GetDevice().CreateBuffer(&bufferDesc, &initData, &mIndexBuffer));

		return true;
	}
}