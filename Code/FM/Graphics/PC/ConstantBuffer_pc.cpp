#include "System/Memory.h"

#include "Graphics/ConstantBuffer.h"
#include "Graphics/Renderer.h"

namespace fm
{
	ConstantBuffer::~ConstantBuffer()
	{
		SafeRelease(mConstantBuffer);
	}

	void ConstantBuffer::Create(const uint32 numBytes)
	{
		Renderer& renderer = Renderer::GetInstance();

		D3D11_BUFFER_DESC bufferDesc;
		MemSet(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.ByteWidth = numBytes;

		Verify(S_OK == renderer.GetDevice().CreateBuffer(&bufferDesc, nullptr, &mConstantBuffer));
	}
}