#include <algorithm>

#include "System/Memory.h"
#include "System/Math/MathUtility.h"

#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"

namespace fm
{
	const DXGI_FORMAT GetPixelFormat(const Texture::eFormat format)
	{
		switch (format)
		{
		case Texture::eFormat::DXT1:
			return DXGI_FORMAT_BC1_TYPELESS;
		case Texture::eFormat::DXT3:
			return DXGI_FORMAT_BC2_TYPELESS;
		case Texture::eFormat::DXT5:
			return DXGI_FORMAT_BC3_TYPELESS;
		default:
			Assert(false, "unknown type");
			break;
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	Texture::Texture(const uint8* surfacedata, const eFormat format, const uint16 width, const uint16 height, 
		const uint8 depth, const uint8 arraysize, const uint8 numMips)
		: mFormat(format)
		, mWidth(width)
		, mHeight(height)
		, mDepth(depth)
		, mArraySize(arraysize)
		, mNumMips(numMips)
	{
		Renderer& renderer = Renderer::GetInstance();

		// links related to how to read DDS files 
		// https://msdn.microsoft.com/en-us/library/windows/desktop/bb943991(v=vs.85).aspx#File_Layout1
		// https://github.com/Microsoft/DirectXTex/tree/master/DDSTextureLoader

		UniquePtr<D3D11_SUBRESOURCE_DATA[]> subResourceData(fmNew(eMemPool::Temp, "texture subresource data") D3D11_SUBRESOURCE_DATA[mArraySize * mNumMips]);

		for (uint32 arryIndex = 0; arryIndex < mArraySize; ++arryIndex)
		{
			for (uint32 mipIndex = 0; mipIndex < mNumMips; ++mipIndex)
			{
				const uint32 subResourceIndex = arryIndex * mNumMips + mipIndex;

				const uint32 numBlocksX = std::max(1, (width >> mipIndex) / 4);
				const uint32 numBlocksY = std::max(1, (height >> mipIndex) / 4);

				const uint32 blockBytes = getBlockByte(mFormat);
				const uint32 rowBytes = numBlocksX * blockBytes;
				const uint32 numRows = numBlocksY;
				const uint32 numBytes = rowBytes * numBlocksY;

				subResourceData[subResourceIndex].pSysMem = reinterpret_cast<const void*>(surfacedata);
				subResourceData[subResourceIndex].SysMemPitch = rowBytes;
				subResourceData[subResourceIndex].SysMemSlicePitch = numBytes;

				surfacedata += numBytes * mDepth;
			}
		}

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = mWidth;
		textureDesc.Height = mHeight;
		textureDesc.MipLevels = mNumMips;
		textureDesc.ArraySize = mArraySize;
		textureDesc.Format = GetPixelFormat(mFormat);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0; 
		// TODO : make cube map set up.
		// textureDesc.MiscFlags  D3D11_RESOURCE_MISC_TEXTURECUBE;

		Verify(S_OK ==  renderer.GetDevice().CreateTexture2D(&textureDesc, subResourceData.get(), &mTexture) );

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
		shaderResourceDesc.Format = GetPixelFormat(mFormat);
		if (mArraySize > 1)
		{
			shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			shaderResourceDesc.Texture2DArray.MipLevels = (!mNumMips) ? -1 : mNumMips;
			shaderResourceDesc.Texture2DArray.ArraySize = mArraySize;
		}
		else
		{
			shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceDesc.Texture2D.MipLevels = (!mNumMips) ? -1 : mNumMips;
		}

		Verify(S_OK == renderer.GetDevice().CreateShaderResourceView(mTexture, &shaderResourceDesc, &mTextureView));
	}

	Texture::~Texture()
	{
		SafeRelease(mTexture);
		SafeRelease(mTextureView);
	}

	const uint32 Texture::getBlockByte(const eFormat format) const
	{
		uint32 blockBytes = 0;
		switch (format)
		{
		case eFormat::DXT1: // DXGI_FORMAT_BC1_TYPELESS
			blockBytes = 8;
			break;

		case eFormat::DXT3: // DXGI_FORMAT_BC2_TYPELESS
		case eFormat::DXT5: // DXGI_FORMAT_BC3_TYPELESS
			blockBytes = 16;
			break;
		}

		return blockBytes;
	}
}